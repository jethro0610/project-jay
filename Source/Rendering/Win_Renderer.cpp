#include "Renderer.h"

#include "../Logging/Logger.h"

void Renderer::Init_P() {
    DXResources* dxResources = resourceManager_->dxResources_;

    width_ = dxResources->width_;
    height_ = dxResources->height_;

    // Need width and height before updating this matrix;
    UpdateProjMatrix(70.0f, 0.5f, 1000.0f);

    // These are all temporary testing for now
    dxResources->LoadVertexShader("StaticVertexShader", VertexShaderType::STATIC);
    dxResources->LoadVertexShader("SkeletalVertexShader", VertexShaderType::SKELETAL);
    dxResources->LoadVertexShader("WorldVertexShader", VertexShaderType::WORLD);
    dxResources->LoadVertexShader("InstancedVertexShader", VertexShaderType::INSTANCED);
    dxResources->LoadPixelShader("DefaultPS");
    dxResources->LoadPixelShader("WorldGrassPS");

    dxResources->LoadTexture("grass_c");
    dxResources->LoadTexture("grass_n");
    dxResources->LoadTexture("bricks_c");
    dxResources->LoadTexture("bricks_n");
    dxResources->LoadTexture("marble_c");
    dxResources->LoadTexture("grid_c");

    MaterialDesc worldMaterialDesc;
    worldMaterialDesc.vertexShader = "WorldVertexShader";
    worldMaterialDesc.pixelShader = "WorldGrassPS";
    worldMaterialDesc.textures[0] = "grass_c";
    worldMaterialDesc.textures[1] = "grass_n";
    worldMaterialDesc.textures[2] = "marble_c";
    worldMaterialDesc.numOfTextures = 3;
    resourceManager_->materials_["worldMaterial"] = worldMaterialDesc;

    MaterialDesc playerMaterial;
    playerMaterial.vertexShader = "StaticVertexShader";
    playerMaterial.pixelShader = "DefaultPS";
    playerMaterial.textures[0] = "bricks_c";
    playerMaterial.textures[1] = "bricks_n";
    playerMaterial.numOfTextures = 2;
    resourceManager_->materials_["playerMaterial"] = playerMaterial;

    MaterialDesc spreadMaterial;
    spreadMaterial.vertexShader = "InstancedVertexShader";
    spreadMaterial.pixelShader = "DefaultPS";
    spreadMaterial.textures[0] = "bricks_c";
    spreadMaterial.textures[1] = "bricks_n";
    spreadMaterial.numOfTextures = 2;
    resourceManager_->materials_["spreadMaterial"] = spreadMaterial;
}

void Renderer::RenderWorld_P() {
    DXResources* dxResources = resourceManager_->dxResources_;
    ID3D11DeviceContext* context = dxResources->context_;

    // OPTIMIZATION: Updating with subresource may be slower than using map
    // likely changing this later
    PerObjectData objectData = {};
    Transform defaultTransform;
    defaultTransform.GetWorldAndNormalMatrix(objectData.worldMat, objectData.normalMat);
    objectData.worldViewProj = GetWorldViewProjection(objectData.worldMat);
    /* context->UpdateSubresource(dxResources->perObjectCBuffer_, 0, nullptr, &objectData, 0, 0); */
    dxResources->UpdateBuffer(dxResources->perObjectCBuffer_, &objectData, sizeof(PerObjectData));

    SetMaterial_P("worldMaterial");

    // Set the vertex and index buffers to be drawn
    context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    UINT vertexStride = sizeof(WorldVertex);
    UINT vertexOffset = 0;
    for (int x = 0; x < MAX_X_COORDINATES; x++)
    for (int y = 0; y < MAX_Y_COORDINATES; y++)
    for (int z = 0; z < MAX_Z_COORDINATES; z++) {
        DXMesh worldMeshResource = dxResources->worldMeshes_[x][y][z];
        context->IASetVertexBuffers(0, 1, &worldMeshResource.vertexBuffer, &vertexStride, &vertexOffset);
        context->IASetIndexBuffer(worldMeshResource.indexBuffer, DXGI_FORMAT_R16_UINT, 0);
        context->DrawIndexed(worldMeshResource.indexCount, 0, 0);
    }
}

void Renderer::RenderEntities_P(Entity* entities, RenderComponents renderComponents) {
    DXResources* dxResources = resourceManager_->dxResources_;
    ID3D11DeviceContext* context = dxResources->context_;

    // Set the vertex and index buffers to be drawn
    context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    UINT vertexStride = sizeof(StaticVertex);
    UINT vertexOffset = 0;

    for (int e = 0; e < MAX_ENTITIES; e++) {
        const Entity& entity = entities[e];
        if (!entities->HasComponents<TransformComponent, StaticModelComponent>())
            continue;

        PerObjectData objectData;
        renderComponents.transformComponents.renderTransform[e].GetWorldAndNormalMatrix(objectData.worldMat, objectData.normalMat);
        objectData.worldViewProj = GetWorldViewProjection(objectData.worldMat);
        dxResources->UpdateBuffer(dxResources->perObjectCBuffer_, &objectData, sizeof(PerObjectData));

        std::string model = renderComponents.staticMeshComponents.model[e];
        StaticModelDesc modelDesc = resourceManager_->staticModels_[model];
        for (int m = 0; m < modelDesc.meshCount; m++) {
            std::string material = renderComponents.staticMeshComponents.materials[e][m];
            SetMaterial_P(material);
            std::string mesh = model + "_" + std::to_string(m);
            DXMesh dxMesh = dxResources->staticMeshes_[mesh];

            context->IASetVertexBuffers(0, 1, &dxMesh.vertexBuffer, &vertexStride, &vertexOffset);
            context->IASetIndexBuffer(dxMesh.indexBuffer, DXGI_FORMAT_R16_UINT, 0);
            context->DrawIndexed(dxMesh.indexCount, 0, 0);
        }
    }
    // OPTIMIZATION: Need to test whether setting the vertex/index buffers or updating subresources takes more time
}


void Renderer::RenderSpread_P(SpreadManager* spreadManager) {
    DXResources* dxResources = resourceManager_->dxResources_;
    ID3D11DeviceContext* context = dxResources->context_;

    PerObjectData objectData;
    Transform defaultTransform;
    defaultTransform.GetWorldAndNormalMatrix(objectData.worldMat, objectData.normalMat);
    objectData.worldViewProj = GetWorldViewProjection(objectData.worldMat);
    dxResources->UpdateBuffer(dxResources->perObjectCBuffer_, &objectData, sizeof(PerObjectData));
    
    context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    UINT strides[2] = { sizeof(StaticVertex), sizeof(InstanceData) };
    UINT offsets[2] = { 0, 0 };
    const std::string material = "spreadMaterial";  
    SetMaterial_P(material);
    DXMesh dxMesh = dxResources->staticMeshes_["st_sphere_0"];
    ID3D11Buffer* buffers[2] = { dxMesh.vertexBuffer, dxResources->spreadBuffer_ };
    context->IASetVertexBuffers(0, 2, buffers, strides, offsets);
    context->IASetIndexBuffer(dxMesh.indexBuffer, DXGI_FORMAT_R16_UINT, 0);
    context->DrawIndexedInstanced(dxMesh.indexCount, spreadManager->count_, 0, 0, 0);
}

void Renderer::Clear_P() {
    DXResources* dxResources = resourceManager_->dxResources_;
    ID3D11DeviceContext* context = dxResources->context_;

    // Clear the render target and depth stencil buffer
    float background_colour[4] = { 0.0f, 0.0f, 0.0f, 1.0f};
    context->ClearRenderTargetView(dxResources->pRenderTarget_, background_colour);
    context->ClearDepthStencilView(dxResources->depthStencilBuffer_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);
}

void Renderer::Present_P() {
    DXResources* dxResources = resourceManager_->dxResources_;
    ID3D11DeviceContext* context = dxResources->context_;

    context->OMSetRenderTargets(1, &dxResources->renderTarget_, nullptr);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    context->VSSetShader(dxResources->vertexShaders_["ScreenQuad"].shader, nullptr, 0);
    context->PSSetShader(dxResources->pixelShaders_["PostProcess"], nullptr, 0);
    context->PSSetShaderResources(0, 1, &dxResources->pRenderTextureResource_);

    context->Draw(4, 0);
    // Set the first parameter to 0 to disable VSync
    dxResources->swapChain_->Present(0, 0);
}

void Renderer::SetMaterial_P(std::string materialName) {
    DXResources* dxResources = resourceManager_->dxResources_;
    ID3D11DeviceContext* context = dxResources->context_;
    MaterialDesc material = resourceManager_->materials_[materialName];

    VSLayout vsLayout = dxResources->vertexShaders_[material.vertexShader];
    context->VSSetShader(vsLayout.shader, nullptr, 0);
    context->IASetInputLayout(vsLayout.layout);

    ID3D11PixelShader* pixelShader = dxResources->pixelShaders_[material.pixelShader];
    context->PSSetShader(pixelShader, nullptr, 0);

    ID3D11ShaderResourceView* textures[MAX_MATERIAL_TEXTURES];
    for (int i = 0; i < material.numOfTextures; i++)
        textures[i] = dxResources->textures_[material.textures[i]];

    context->PSSetShaderResources(0, material.numOfTextures, textures);
    context->PSSetSamplers(0, 1, &dxResources->textureSampler_);
}

void Renderer::SetFrameData_P() {
    DXResources* dxResources = resourceManager_->dxResources_;
    ID3D11DeviceContext* context = dxResources->context_;
    
    context->OMSetRenderTargets(1, &dxResources->pRenderTarget_, dxResources->depthStencilBuffer_);
    PerFrameData frameData = {};
    frameData.cameraPos = camera_->transform_.position_;
    frameData.time = 0.0f; // TODO: Set the time with a function input
	dxResources->UpdateBuffer(dxResources->perFrameCBuffer_, &frameData, sizeof(PerFrameData)); 
}
