#include "Renderer.h"

Renderer::Renderer(DXResources* dxResources){
    dxResources_ = dxResources;

    dxResources_->LoadVertexShader("StaticVertexShader", false);
    dxResources_->LoadVertexShader("SkeletalVertexShader", true);
    dxResources_->LoadPixelShader("PixelShader");
    dxResources_->LoadModel("st_toruscone");
    dxResources_->LoadTexture("testTex");
    dxResources_->LoadTexture("testNorm");

    width_ = dxResources_->width_;
    height_ = dxResources_->height_;

    Init();
}

Renderer::~Renderer() {
   
}

void Renderer::Render_P() {
    ID3D11DeviceContext* context = dxResources_->context_;
    PerObjectData objectData = {};
    mat4 worldMat;
    Temp_GetWorldAndNormalMatrix(worldMat, objectData.normalMat);
    objectData.worldViewProj = GetWorldViewProjection(worldMat);
    
    // Update the per frame constant buffer
    context->UpdateSubresource(dxResources_->perObjectCBuffer_, 0, nullptr, &objectData, 0, 0);

    // Clear the render target and depth stencil buffer
    float background_colour[4] = { 0x64 / 255.0f, 0x95 / 255.0f, 0xED / 255.0f, 1.0f };
    context->ClearRenderTargetView(dxResources_->renderTarget_, background_colour);
    context->ClearDepthStencilView(dxResources_->depthStencilBuffer_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);

    // Get the vertex and index variables
    MeshResource meshResource0 = dxResources_->staticMeshes_["st_toruscone_0"];
    MeshResource meshResource1 = dxResources_->staticMeshes_["st_toruscone_1"];
    UINT vertexStride = sizeof(StaticVertex);
    UINT vertexOffset = 0;

    // Get the vertex shader
    VSResource vsResource = dxResources_->vertexShaders_["StaticVertexShader"];

    // Get the pixel shader
    PSResource psResource = dxResources_->pixelShaders_["PixelShader"];

    // Set the vertex shader and update its constant buffers
    context->VSSetShader(vsResource.shader, nullptr, 0);

    // Set the pixel shader
    context->PSSetShader(psResource.shader, nullptr, 0);

    // Set the texture and texture sampler
    ID3D11ShaderResourceView* resources[2];
    resources[0] = dxResources_->textures_["testTex"].texture;
    resources[1] = dxResources_->textures_["testNorm"].texture;
    context->PSSetShaderResources(0, 2, resources);
    context->PSSetSamplers(0, 1, &dxResources_->textureSampler_);

    // Set the vertex and index buffers to be drawn
    context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->IASetInputLayout(vsResource.layout);
    context->IASetVertexBuffers(0, 1, &meshResource0.vertexBuffer, &vertexStride, &vertexOffset);
    context->IASetIndexBuffer(meshResource0.indexBuffer, DXGI_FORMAT_R16_UINT, 0);
    context->DrawIndexed(meshResource0.indexCount, 0, 0);

    // Set the vertex and index buffers to be drawn
    context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->IASetInputLayout(vsResource.layout);
    context->IASetVertexBuffers(0, 1, &meshResource1.vertexBuffer, &vertexStride, &vertexOffset);
    context->IASetIndexBuffer(meshResource1.indexBuffer, DXGI_FORMAT_R16_UINT, 0);
    context->DrawIndexed(meshResource1.indexCount, 0, 0);

    // Set the first parameter to 0 to disable VSync
    dxResources_->swapChain_->Present(1, 0);
}