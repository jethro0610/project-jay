#include "Renderer.h"
#include "../Game/Camera.h"
#include "../Resource/DXResources.h"
#include "../Game/Entity/Entity.h"
#include "../Constants/GameConstants.h"
#include "../Game/PlayerController.h"
#include "../Helpers/EntityHelpers.h"
#include "../Resource/ResourceManager.h"
#include "../Game/World/SpreadManager.h"
#include "../Types/Transform.h"
#include "../Game/Components/MeterComponent.h"
#include "../Game/Components/StaticModelComponent.h"
#include "../Game/Time.h"
#include "../Game/Components/TransformComponent.h"

Renderer::Renderer(ResourceManager& resourceManager):
    resourceManager_(resourceManager),
    dxResources_(resourceManager_.dxResources_),
    context_(dxResources_.context_),
    viewMatrix_(0),
    projMatrix_(0),
    camera_(nullptr)
{
    width_ = dxResources_.width_;
    height_ = dxResources_.height_;

    // Need width and height before updating this matrix;
    UpdateProjMatrix(70.0f, 0.5f, 1000.0f);

    dxResources_.LoadVertexShader("StaticVS", VertexShaderType::STATIC);
    dxResources_.LoadVertexShader("SkeletalVS", VertexShaderType::SKELETAL);
    dxResources_.LoadVertexShader("WorldVS", VertexShaderType::WORLD);
    dxResources_.LoadVertexShader("InstanceVS", VertexShaderType::INSTANCE);
    dxResources_.LoadVertexShader("ScreenQuadVS", VertexShaderType::STATIC);
    dxResources_.LoadVertexShader("ScreenBarVS", VertexShaderType::STATIC);

    dxResources_.LoadPixelShader("DefaultPS");
    dxResources_.LoadPixelShader("GrassPS");
    dxResources_.LoadPixelShader("PostProcessPS");
    dxResources_.LoadPixelShader("BarPS");

    dxResources_.LoadTexture("grass_c");
    dxResources_.LoadTexture("grass_n");
    dxResources_.LoadTexture("bricks_c");
    dxResources_.LoadTexture("bricks_n");
    dxResources_.LoadTexture("marble_c");
    dxResources_.LoadTexture("grid_c");

    MaterialDesc worldMaterialDesc;
    worldMaterialDesc.vertexShader = "WorldVS";
    worldMaterialDesc.pixelShader = "GrassPS";
    worldMaterialDesc.textures[0] = "grass_c";
    worldMaterialDesc.textures[1] = "grass_n";
    worldMaterialDesc.textures[2] = "marble_c";
    worldMaterialDesc.numOfTextures = 3;
    resourceManager_.materials_["worldMaterial"] = worldMaterialDesc;

    MaterialDesc playerMaterial;
    playerMaterial.vertexShader = "StaticVS";
    playerMaterial.pixelShader = "DefaultPS";
    playerMaterial.textures[0] = "bricks_c";
    playerMaterial.textures[1] = "bricks_n";
    playerMaterial.numOfTextures = 2;
    resourceManager_.materials_["playerMaterial"] = playerMaterial;

    MaterialDesc spreadMaterial;
    spreadMaterial.vertexShader = "InstanceVS";
    spreadMaterial.pixelShader = "DefaultPS";
    spreadMaterial.textures[0] = "bricks_c";
    spreadMaterial.textures[1] = "bricks_n";
    spreadMaterial.numOfTextures = 2;
    resourceManager_.materials_["spreadMaterial"] = spreadMaterial;
}

void Renderer::RenderWorld_P(World& world) {
    context_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Since no transformations are being done, just use a 
    // transform initailized to 0
    PerObjectData objectData = {};
    Transform defaultTransform;
    defaultTransform.GetWorldAndNormalMatrix(objectData.worldMat, objectData.normalMat);
    objectData.worldViewProj = GetWorldViewProjection(objectData.worldMat);
    dxResources_.UpdateBuffer(dxResources_.perObjectCBuffer_, &objectData, sizeof(PerObjectData));

    SetMaterial_P("worldMaterial");

    UINT strides[1] = { sizeof(WorldVertex) };
    UINT offsets[1] = { 0 };

    for (int x = 0; x < MAX_X_CHUNKS; x++)
    for (int y = 0; y < MAX_Y_CHUNKS; y++)
    for (int z = 0; z < MAX_Z_CHUNKS; z++) {
        WorldMesh& worldMeshResource = dxResources_.worldMeshes_[x][y][z];
        ID3D11Buffer* buffers[1] = { worldMeshResource.vertexBuffer };
        context_->IASetVertexBuffers(0, 1, buffers, strides, offsets);
        context_->Draw(worldMeshResource.vertexCount, 0);
    }
}

EntityKey constexpr key = GetEntityKey<StaticModelComponent, TransformComponent>();
void Renderer::RenderEntities_P(
    Entity* entities, 
    StaticModelComponent& staticModelComponent,
    TransformComponent& transformComponent
) {
    context_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Set the vertex and index buffers to be drawn
    UINT vertexStride = sizeof(StaticVertex);
    UINT vertexOffset = 0;

    for (int e = 0; e < MAX_ENTITIES; e++) {
        const Entity& entity = entities[e];
        if (!entity.alive_)
            continue;
        if (!entity.MatchesKey(key))
            continue;

        PerObjectData objectData;
        transformComponent.renderTransform[e].GetWorldAndNormalMatrix(objectData.worldMat, objectData.normalMat);
        objectData.worldViewProj = GetWorldViewProjection(objectData.worldMat);
        dxResources_.UpdateBuffer(dxResources_.perObjectCBuffer_, &objectData, sizeof(PerObjectData));

        std::string model = staticModelComponent.model[e];
        StaticModelDesc modelDesc = resourceManager_.staticModels_[model];
        for (int m = 0; m < modelDesc.meshCount; m++) {
            std::string material = staticModelComponent.materials[e][m];
            SetMaterial_P(material);
            std::string mesh = model + "_" + std::to_string(m);
            DXMesh dxMesh = dxResources_.staticMeshes_[mesh];

            context_->IASetVertexBuffers(0, 1, &dxMesh.vertexBuffer, &vertexStride, &vertexOffset);
            context_->IASetIndexBuffer(dxMesh.indexBuffer, DXGI_FORMAT_R16_UINT, 0);
            context_->DrawIndexed(dxMesh.indexCount, 0, 0);
        }
    }
    // OPTIMIZATION: Need to test whether setting the vertex/index buffers or updating subresources takes more time
}

void Renderer::RenderSpread_P(SpreadManager& spreadManager) {
    context_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    PerObjectData objectData;
    Transform defaultTransform;
    defaultTransform.GetWorldAndNormalMatrix(objectData.worldMat, objectData.normalMat);
    objectData.worldViewProj = GetWorldViewProjection(objectData.worldMat);
    dxResources_.UpdateBuffer(dxResources_.perObjectCBuffer_, &objectData, sizeof(PerObjectData));

    UINT strides[2] = { sizeof(StaticVertex), sizeof(InstanceData) };
    UINT offsets[2] = { 0, 0 };

    const std::string model = "st_sphere";
    StaticModelDesc modelDesc = resourceManager_.staticModels_["st_sphere"];
    const std::string material = "spreadMaterial";  
    SetMaterial_P(material);

    for (int m = 0; m < modelDesc.meshCount; m++) {
        std::string mesh = model + "_" + std::to_string(m);
        DXMesh dxMesh = dxResources_.staticMeshes_[mesh];
        context_->IASetIndexBuffer(dxMesh.indexBuffer, DXGI_FORMAT_R16_UINT, 0);

        // TODO: Render only on visible chunks 
        for (int x = 0; x < MAX_X_CHUNKS; x++)
        for (int z = 0; z < MAX_Z_CHUNKS; z++) {
            ID3D11Buffer* buffers[2] = { dxMesh.vertexBuffer, dxResources_.spreadBuffers_[x][z] };
            context_->IASetVertexBuffers(0, 2, buffers, strides, offsets);
            context_->DrawIndexedInstanced(dxMesh.indexCount, spreadManager.spreadChunks_[x][z].count, 0, 0, 0);
        }
    }
}

void Renderer::RenderSpreadOrbs_P(SpreadManager& spreadManager) {

}

void Renderer::RenderPostProcess_P() {
    context_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    context_->VSSetShader(dxResources_.vertexShaders_["ScreenQuadVS"].shader, nullptr, 0);
    context_->PSSetShader(dxResources_.pixelShaders_["PostProcessPS"], nullptr, 0);
    context_->PSSetShaderResources(0, 1, &dxResources_.pRenderTextureResource_);
    context_->Draw(4, 0);
}

void Renderer::RenderUI_P(MeterComponent& meterComponent) {
    context_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    PerUIData uiData = { meterComponent.meter[PLAYER_ENTITY] / float(meterComponent.maxMeter[PLAYER_ENTITY]) };
    dxResources_.UpdateBuffer(dxResources_.perUICBuffer_, &uiData, sizeof(PerUIData));
    context_->VSSetShader(dxResources_.vertexShaders_["ScreenBarVS"].shader, nullptr, 0);
    context_->PSSetShader(dxResources_.pixelShaders_["BarPS"], nullptr, 0);
    context_->Draw(4, 0);
}

#ifdef _DEBUG
void Renderer::RenderScreenText_P() {
    if (!ScreenText::IsEnabled())
        return;
    context_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    dxResources_.UpdateBuffer(dxResources_.textBuffer_, ScreenText::GetLines(), sizeof(TextData) * MAX_LINES * CHARS_PER_LINE); 

    context_->VSSetShader(dxResources_.textVS_, nullptr, 0);
    context_->IASetInputLayout(dxResources_.textVSLayout_);
    context_->PSSetShader(dxResources_.textPS_, nullptr, 0);
    context_->PSSetShaderResources(0, 1, &dxResources_.textTexture_);

    UINT strides[1] = { sizeof(TextData) };
    UINT offsets[1] = { 0 };
    ID3D11Buffer* buffers[1] = { dxResources_.textBuffer_ };
    context_->IASetVertexBuffers(0, 1, buffers, strides, offsets);
    context_->DrawInstanced(4, MAX_LINES * CHARS_PER_LINE, 0, 0);
}
#endif

void Renderer::Clear_P() {
    // Clear the render target and depth stencil buffer
    float background_colour[4] = { 0.0f, 0.0f, 0.0f, 1.0f};
    context_->ClearRenderTargetView(dxResources_.pRenderTarget_, background_colour);
    context_->ClearDepthStencilView(dxResources_.depthStencilBuffer_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);
}

void Renderer::Present_P() {
    dxResources_.swapChain_->Present(0, 0);
}

void Renderer::SetMaterial_P(std::string materialName) {
    MaterialDesc material = resourceManager_.materials_[materialName];

    VSLayout vsLayout = dxResources_.vertexShaders_[material.vertexShader];
    context_->VSSetShader(vsLayout.shader, nullptr, 0);
    context_->IASetInputLayout(vsLayout.layout);

    ID3D11PixelShader* pixelShader = dxResources_.pixelShaders_[material.pixelShader];
    context_->PSSetShader(pixelShader, nullptr, 0);

    ID3D11ShaderResourceView* textures[MAX_MATERIAL_TEXTURES];
    for (int i = 0; i < material.numOfTextures; i++)
        textures[i] = dxResources_.textures_[material.textures[i]];

    context_->PSSetShaderResources(0, material.numOfTextures, textures);
    context_->PSSetSamplers(0, 1, &dxResources_.textureSampler_);
}

void Renderer::SetFrameData_P() {
    PerFrameData frameData = {};
    frameData.aspectRatio = float(width_) / height_;
    frameData.cameraPos = camera_->transform_.position_;
    frameData.time = Time::GetTime(); 
    frameData.viewMatrix = viewMatrix_;
	dxResources_.UpdateBuffer(dxResources_.perFrameCBuffer_, &frameData, sizeof(PerFrameData)); 
}

void Renderer::EnableBlend_P() {
    UINT sampleMask = 0xffffffff;
    float blendFactor[] = {0.75f, 0.75f, 0.75f, 1.0f};
    context_->OMSetBlendState(dxResources_.alphaBlendState_, blendFactor, sampleMask);
}

void Renderer::DisableBlend_P() {
    UINT sampleMask = 0xffffffff;
    context_->OMSetBlendState(dxResources_.noBlendState_, 0, sampleMask);
}

void Renderer::SetRenderTargetWorld_P() {
    ID3D11ShaderResourceView *const pSRV[1] = { NULL };
    context_->PSSetShaderResources(0, 1, pSRV); // Unbind the render target from Post Process pixel shader
    context_->OMSetRenderTargets(1, &dxResources_.pRenderTarget_, dxResources_.depthStencilBuffer_);
}

void Renderer::SetRenderTargetScreen_P() {
    context_->OMSetRenderTargets(1, &dxResources_.renderTarget_, nullptr);
}
