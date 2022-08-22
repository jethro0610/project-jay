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
    dxResources->LoadPixelShader("PixelShader");
    dxResources->LoadTexture("testTex");
    dxResources->LoadTexture("testNorm");
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

    // Get the vertex shader
    VSResource vsResource = dxResources->vertexShaders_["WorldVertexShader"];
    context->VSSetShader(vsResource.shader, nullptr, 0);

    // Get the pixel shader
    PSResource psResource = dxResources->pixelShaders_["PixelShader"];

    // Set the vertex shader and update its constant buffers
    context->VSSetShader(vsResource.shader, nullptr, 0);

    // Set the pixel shader
    context->PSSetShader(psResource.shader, nullptr, 0);

    // Set the vertex and index buffers to be drawn
    context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->IASetInputLayout(vsResource.layout);
    UINT vertexStride = sizeof(WorldVertex);
    UINT vertexOffset = 0;
    for (int x = 0; x < MAX_X_COORDINATES; x++)
    for (int y = 0; y < MAX_Y_COORDINATES; y++)
    for (int z = 0; z < MAX_Z_COORDINATES; z++) {
        MeshResource worldMeshResource = dxResources->worldMeshes_[x][y][z];
        context->IASetVertexBuffers(0, 1, &worldMeshResource.vertexBuffer, &vertexStride, &vertexOffset);
        context->IASetIndexBuffer(worldMeshResource.indexBuffer, DXGI_FORMAT_R16_UINT, 0);
        context->DrawIndexed(worldMeshResource.indexCount, 0, 0);
    }
}

void Renderer::RenderStaticMeshes_P(RenderComponents renderComponents, const StaticModelRenderList& staticModelRenderList) {
    DXResources* dxResources = resourceManager_->dxResources_;
    ID3D11DeviceContext* context = dxResources->context_;

    // Get the vertex shader
    VSResource vsResource = dxResources->vertexShaders_["StaticVertexShader"];
    context->VSSetShader(vsResource.shader, nullptr, 0);

    // Get the pixel shader
    PSResource psResource = dxResources->pixelShaders_["PixelShader"];

    // Set the vertex shader and update its constant buffers
    context->VSSetShader(vsResource.shader, nullptr, 0);

    // Set the pixel shader
    context->PSSetShader(psResource.shader, nullptr, 0);

    // Set the vertex and index buffers to be drawn
    context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->IASetInputLayout(vsResource.layout);
    UINT vertexStride = sizeof(StaticVertex);
    UINT vertexOffset = 0;

    // Iterate through every model assert in the list
    for (auto modelItr = staticModelRenderList.begin(); modelItr != staticModelRenderList.end(); modelItr++) {
        std::string model = modelItr->first;
        StaticModelDescription description = resourceManager_->loadedStaticModels_[model];

        // Iterate through every mesh of the model
        for (int i = 0; i < description.meshCount; i++) {
            std::string meshName = model + "_" + std::to_string(i);
            MeshResource meshResource = dxResources->staticMeshes_[meshName];

            context->IASetVertexBuffers(0, 1, &meshResource.vertexBuffer, &vertexStride, &vertexOffset);
            context->IASetIndexBuffer(meshResource.indexBuffer, DXGI_FORMAT_R16_UINT, 0);

            // Draw every entity with the model
            for (auto entityItr = modelItr->second.begin(); entityItr != modelItr->second.end(); entityItr++) {
                PerObjectData objectData;
                mat4 worldMatrix;
                renderComponents.transformComponents->renderTransform[*entityItr].GetWorldAndNormalMatrix(worldMatrix, objectData.normalMat);
                objectData.worldViewProj = GetWorldViewProjection(worldMatrix);
                context->UpdateSubresource(dxResources->perObjectCBuffer_, 0, nullptr, &objectData, 0, 0);
                context->DrawIndexed(meshResource.indexCount, 0, 0);
            }
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