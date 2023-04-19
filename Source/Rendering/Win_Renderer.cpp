#include "Renderer.h"
#include "../Game/Camera.h"
#include "../Resource/DXResources.h"
#include "../Game/Entity/EntityManager.h"
#include "../Resource/ResourceManager.h"
#include "../Game/World/SpreadManager.h"
#include "../Types/Transform.h"
#include "../Logging/Logger.h"

Renderer::Renderer(ResourceManager& resourceManager):
    resourceManager_(resourceManager),
    viewMatrix_(0),
    projMatrix_(0),
    camera_(nullptr)
{
    DXResources& dxResources = resourceManager_.dxResources_;

    width_ = dxResources.width_;
    height_ = dxResources.height_;

    // Need width and height before updating this matrix;
    UpdateProjMatrix(70.0f, 0.5f, 1000.0f);

    // These are all temporary testing for now
    dxResources.LoadVertexShader("StaticVertexShader", VertexShaderType::STATIC);
    dxResources.LoadVertexShader("SkeletalVertexShader", VertexShaderType::SKELETAL);
    dxResources.LoadVertexShader("WorldVertexShader", VertexShaderType::WORLD);
    dxResources.LoadVertexShader("InstancedVertexShader", VertexShaderType::SPREAD);

    dxResources.LoadPixelShader("DefaultPS");
    dxResources.LoadPixelShader("WorldGrassPS");
    dxResources.LoadPixelShader("TextureOnly");

    dxResources.LoadTexture("grass_c");
    dxResources.LoadTexture("grass_n");
    dxResources.LoadTexture("bricks_c");
    dxResources.LoadTexture("bricks_n");
    dxResources.LoadTexture("marble_c");
    dxResources.LoadTexture("grid_c");

    MaterialDesc worldMaterialDesc;
    worldMaterialDesc.vertexShader = "WorldVertexShader";
    worldMaterialDesc.pixelShader = "WorldGrassPS";
    worldMaterialDesc.textures[0] = "grass_c";
    worldMaterialDesc.textures[1] = "grass_n";
    worldMaterialDesc.textures[2] = "marble_c";
    worldMaterialDesc.numOfTextures = 3;
    resourceManager_.materials_["worldMaterial"] = worldMaterialDesc;

    MaterialDesc playerMaterial;
    playerMaterial.vertexShader = "StaticVertexShader";
    playerMaterial.pixelShader = "DefaultPS";
    playerMaterial.textures[0] = "bricks_c";
    playerMaterial.textures[1] = "bricks_n";
    playerMaterial.numOfTextures = 2;
    resourceManager_.materials_["playerMaterial"] = playerMaterial;

    MaterialDesc spreadMaterial;
    spreadMaterial.vertexShader = "InstancedVertexShader";
    spreadMaterial.pixelShader = "DefaultPS";
    spreadMaterial.textures[0] = "bricks_c";
    spreadMaterial.textures[1] = "bricks_n";
    spreadMaterial.numOfTextures = 2;
    resourceManager_.materials_["spreadMaterial"] = spreadMaterial;
}

void Renderer::RenderWorld_P(World& world) {
    DXResources& dxResources = resourceManager_.dxResources_;
    ID3D11DeviceContext* context = dxResources.context_;

    // OPTIMIZATION: Updating with subresource may be slower than using map
    // likely changing this later
    PerObjectData objectData = {};
    Transform defaultTransform;
    defaultTransform.GetWorldAndNormalMatrix(objectData.worldMat, objectData.normalMat);
    objectData.worldViewProj = GetWorldViewProjection(objectData.worldMat);
    dxResources.UpdateBuffer(dxResources.perObjectCBuffer_, &objectData, sizeof(PerObjectData));

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    SetMaterial_P("worldMaterial");

    UINT strides[1] = { sizeof(WorldVertex) };
    UINT offsets[1] = { 0 };

    for (int x = 0; x < MAX_X_CHUNKS; x++)
    for (int y = 0; y < MAX_Y_CHUNKS; y++)
    for (int z = 0; z < MAX_Z_CHUNKS; z++) {
        WorldMesh& worldMeshResource = dxResources.worldMeshes_[x][y][z];
        ID3D11Buffer* buffers[1] = { worldMeshResource.vertexBuffer };
        context->IASetVertexBuffers(0, 1, buffers, strides, offsets);
        context->Draw(worldMeshResource.vertexCount, 0);
    }
}

void Renderer::RenderEntities_P(EntityManager& entityManager) {
    DXResources& dxResources = resourceManager_.dxResources_;
    ID3D11DeviceContext* context = dxResources.context_;

    StaticModelComponent& staticModelComponent = entityManager.staticModelComponent_;
    TransformComponent& transformComponent = entityManager.transformComponent_;

    // Set the vertex and index buffers to be drawn
    context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    UINT vertexStride = sizeof(StaticVertex);
    UINT vertexOffset = 0;

    const Entity* entities = entityManager.entities_;
    for (int e = 0; e < MAX_ENTITIES; e++) {
        const Entity& entity = entities[e];
        if (!entity.alive_)
            continue;
        if (!entities[e].HasComponents({
            staticModelComponent,
            transformComponent 
        })) continue;

        PerObjectData objectData;
        transformComponent.renderTransform[e].GetWorldAndNormalMatrix(objectData.worldMat, objectData.normalMat);
        objectData.worldViewProj = GetWorldViewProjection(objectData.worldMat);
        dxResources.UpdateBuffer(dxResources.perObjectCBuffer_, &objectData, sizeof(PerObjectData));

        std::string model = staticModelComponent.model[e];
        StaticModelDesc modelDesc = resourceManager_.staticModels_[model];
        for (int m = 0; m < modelDesc.meshCount; m++) {
            std::string material = staticModelComponent.materials[e][m];
            SetMaterial_P(material);
            std::string mesh = model + "_" + std::to_string(m);
            DXMesh dxMesh = dxResources.staticMeshes_[mesh];

            context->IASetVertexBuffers(0, 1, &dxMesh.vertexBuffer, &vertexStride, &vertexOffset);
            context->IASetIndexBuffer(dxMesh.indexBuffer, DXGI_FORMAT_R16_UINT, 0);
            context->DrawIndexed(dxMesh.indexCount, 0, 0);
        }
    }
    // OPTIMIZATION: Need to test whether setting the vertex/index buffers or updating subresources takes more time
}

void Renderer::RenderSpread_P(SpreadManager& spreadManager) {
    DXResources& dxResources = resourceManager_.dxResources_;
    ID3D11DeviceContext* context = dxResources.context_;

    PerObjectData objectData;
    Transform defaultTransform;
    defaultTransform.GetWorldAndNormalMatrix(objectData.worldMat, objectData.normalMat);
    objectData.worldViewProj = GetWorldViewProjection(objectData.worldMat);
    dxResources.UpdateBuffer(dxResources.perObjectCBuffer_, &objectData, sizeof(PerObjectData));

    context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    UINT strides[2] = { sizeof(StaticVertex), sizeof(SpreadInstance) };
    UINT offsets[2] = { 0, 0 };

    const std::string model = "st_sphere";
    StaticModelDesc modelDesc = resourceManager_.staticModels_["st_sphere"];
    const std::string material = "spreadMaterial";  
    SetMaterial_P(material);

    for (int m = 0; m < modelDesc.meshCount; m++) {
        std::string mesh = model + "_" + std::to_string(m);
        DXMesh dxMesh = dxResources.staticMeshes_[mesh];
        context->IASetIndexBuffer(dxMesh.indexBuffer, DXGI_FORMAT_R16_UINT, 0);

        // TODO: Render only on visible chunks 
        for (int x = 0; x < MAX_X_CHUNKS; x++)
        for (int z = 0; z < MAX_Z_CHUNKS; z++) {
            ID3D11Buffer* buffers[2] = { dxMesh.vertexBuffer, dxResources.spreadBuffers_[x][z] };
            context->IASetVertexBuffers(0, 2, buffers, strides, offsets);
            context->DrawIndexedInstanced(dxMesh.indexCount, spreadManager.spreadChunks_[x][z].count, 0, 0, 0);
        }
    }
}

void Renderer::RenderPostProcess_P() {
    DXResources& dxResources = resourceManager_.dxResources_;
    ID3D11DeviceContext* context = dxResources.context_;

    context->OMSetRenderTargets(1, &dxResources.renderTarget_, nullptr);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    context->VSSetShader(dxResources.vertexShaders_["ScreenQuad"].shader, nullptr, 0);
    context->PSSetShader(dxResources.pixelShaders_["PostProcess"], nullptr, 0);
    context->PSSetShaderResources(0, 1, &dxResources.pRenderTextureResource_);

    context->Draw(4, 0);
}

#ifdef _DEBUG
void Renderer::RenderScreenText_P() {
    if (!ScreenText::IsEnabled())
        return;

    DXResources& dxResources = resourceManager_.dxResources_;
    ID3D11DeviceContext* context = dxResources.context_;
    
    dxResources.UpdateBuffer(dxResources.textBuffer_, ScreenText::GetLines(), sizeof(TextData) * MAX_LINES * CHARS_PER_LINE); 

    UINT sampleMask = 0xffffffff;
    float blendFactor[] = {0.75f, 0.75f, 0.75f, 1.0f};

    context->OMSetBlendState(dxResources.alphaBlendState_, blendFactor, sampleMask);
    context->OMSetRenderTargets(1, &dxResources.renderTarget_, nullptr);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    context->VSSetShader(dxResources.textVS_, nullptr, 0);
    context->IASetInputLayout(dxResources.textVSLayout_);
    context->PSSetShader(dxResources.textPS_, nullptr, 0);
    context->PSSetShaderResources(0, 1, &dxResources.textTexture_);

    UINT strides[1] = { sizeof(TextData) };
    UINT offsets[1] = { 0 };
    ID3D11Buffer* buffers[1] = { dxResources.textBuffer_ };
    context->IASetVertexBuffers(0, 1, buffers, strides, offsets);

    context->DrawInstanced(4, MAX_LINES * CHARS_PER_LINE, 0, 0);
}
#endif

void Renderer::Clear_P() {
    DXResources& dxResources = resourceManager_.dxResources_;
    ID3D11DeviceContext* context = dxResources.context_;

    // Clear the render target and depth stencil buffer
    float background_colour[4] = { 0.0f, 0.0f, 0.0f, 1.0f};
    context->ClearRenderTargetView(dxResources.pRenderTarget_, background_colour);
    context->ClearDepthStencilView(dxResources.depthStencilBuffer_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);
}

void Renderer::Present_P() {
    DXResources& dxResources = resourceManager_.dxResources_;
    dxResources.swapChain_->Present(0, 0);
}

void Renderer::SetMaterial_P(std::string materialName) {
    DXResources& dxResources = resourceManager_.dxResources_;
    ID3D11DeviceContext* context = dxResources.context_;
    MaterialDesc material = resourceManager_.materials_[materialName];

    VSLayout vsLayout = dxResources.vertexShaders_[material.vertexShader];
    context->VSSetShader(vsLayout.shader, nullptr, 0);
    context->IASetInputLayout(vsLayout.layout);

    ID3D11PixelShader* pixelShader = dxResources.pixelShaders_[material.pixelShader];
    context->PSSetShader(pixelShader, nullptr, 0);

    ID3D11ShaderResourceView* textures[MAX_MATERIAL_TEXTURES];
    for (int i = 0; i < material.numOfTextures; i++)
        textures[i] = dxResources.textures_[material.textures[i]];

    context->PSSetShaderResources(0, material.numOfTextures, textures);
    context->PSSetSamplers(0, 1, &dxResources.textureSampler_);
}

void Renderer::SetFrameData_P() {
    DXResources& dxResources = resourceManager_.dxResources_;
    ID3D11DeviceContext* context = dxResources.context_;
    
    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    UINT sampleMask = 0xffffffff;

    context->OMSetBlendState(dxResources.noBlendState_, 0, sampleMask);

    ID3D11ShaderResourceView *const pSRV[1] = { NULL };
    context->PSSetShaderResources(0, 1, pSRV); // Unbind the render target from Post Process pixel shader
    context->OMSetRenderTargets(1, &dxResources.pRenderTarget_, dxResources.depthStencilBuffer_);

    PerFrameData frameData = {};
    frameData.aspectRatio = float(width_) / height_;
    frameData.cameraPos = camera_->transform_.position_;
    frameData.time = 0.0f; // TODO: Set the time with a function input
	dxResources.UpdateBuffer(dxResources.perFrameCBuffer_, &frameData, sizeof(PerFrameData)); 
}

void Renderer::RenderTextureToScreen_P() {
    DXResources& dxResources = resourceManager_.dxResources_;
    ID3D11DeviceContext* context = dxResources.context_;

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    context->VSSetShader(dxResources.vertexShaders_["ScreenQuad"].shader, nullptr, 0);
    context->PSSetShader(dxResources.pixelShaders_["TextureOnly"], nullptr, 0);
    context->PSSetShaderResources(0, 1, &dxResources.noiseTextureSRV_);

    context->Draw(4, 0);
}
