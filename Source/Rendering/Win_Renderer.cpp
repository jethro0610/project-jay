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
}

void Renderer::RenderWorld_P() {
    DXResources* dxResources = resourceManager_->dxResources_;
    ID3D11DeviceContext* context = dxResources->context_;

    // OPTIMIZATION: Updating with subresource may be slower than using map
    // likely changing this later
    PerObjectData objectData = {};
    Transform defaultTransform;
    mat4 worldMatrix;
    defaultTransform.GetWorldAndNormalMatrix(worldMatrix, objectData.normalMat);
    objectData.worldViewProj = GetWorldViewProjection(worldMatrix);
    context->UpdateSubresource(dxResources->perObjectCBuffer_, 0, nullptr, &objectData, 0, 0);

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
        mat4 worldMatrix;
        renderComponents.transformComponents.renderTransform[e].GetWorldAndNormalMatrix(worldMatrix, objectData.normalMat);
        objectData.worldViewProj = GetWorldViewProjection(worldMatrix);
        context->UpdateSubresource(dxResources->perObjectCBuffer_, 0, nullptr, &objectData, 0, 0);

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

void Renderer::Clear_P() {
    DXResources* dxResources = resourceManager_->dxResources_;
    ID3D11DeviceContext* context = dxResources->context_;

    // Clear the render target and depth stencil buffer
    float background_colour[4] = { 0x64 / 255.0f, 0x95 / 255.0f, 0xED / 255.0f, 1.0f };
    context->ClearRenderTargetView(dxResources->renderTarget_, background_colour);
    context->ClearDepthStencilView(dxResources->depthStencilBuffer_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);
}

void Renderer::Present_P() {
    // Set the first parameter to 0 to disable VSync
    resourceManager_->dxResources_->swapChain_->Present(0, 0);
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