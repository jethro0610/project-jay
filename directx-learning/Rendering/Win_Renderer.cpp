#include "Renderer.h"
#include "../Logger.h"
Renderer::Renderer(DirectXLayer* dxLayer){
    dxLayer_ = dxLayer;

    dxLayer_->LoadVertexShader("StaticVertexShader", false);
    dxLayer_->LoadVertexShader("SkeletalVertexShader", true);
    dxLayer_->LoadPixelShader("PixelShader");
    dxLayer_->LoadModel("st_toruscone");
    dxLayer_->LoadModel("sk_test", true);
    dxLayer_->LoadTexture("testTex");
    dxLayer_->LoadTexture("testNorm");

    width_ = dxLayer->width_;
    height_ = dxLayer->height_;

    Init();
}

Renderer::~Renderer() {
   
}

void Renderer::PlatformRender() {
    ID3D11DeviceContext* context = dxLayer_->context_;
    PerObjectData objectData = {};
    mat4 worldMat;
    Temp_GetWorldAndNormalMatrix(worldMat, objectData.normalMat);
    objectData.worldViewProj = GetWorldViewProjection(worldMat);
    
    // Update the per frame constant buffer
    context->UpdateSubresource(dxLayer_->perObjectCBuffer_, 0, nullptr, &objectData, 0, 0);

    // Update the per skeleton constant buffer
    PerSkeletonData skeletonData = {};
    mat4 globalJointTransforms[32];
    dxLayer_->testSkeleton_->CalculateGlobalJointTransforms(globalJointTransforms);

    for (int i = 0; i < dxLayer_->testSkeleton_->joints_.size(); i++) {
        skeletonData.jointMatrices[i] = globalJointTransforms[i] * dxLayer_->testSkeleton_->joints_[i].inverseBindMatrix_;
    }
    context->UpdateSubresource(dxLayer_->perSkeletonCBuffer_, 0, nullptr, &skeletonData, 0, 0);

    // Clear the render target and depth stencil buffer
    float background_colour[4] = { 0x64 / 255.0f, 0x95 / 255.0f, 0xED / 255.0f, 1.0f };
    context->ClearRenderTargetView(dxLayer_->renderTarget_, background_colour);
    context->ClearDepthStencilView(dxLayer_->depthStencilBuffer_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);

    // Get the vertex and index variables
    MeshResource meshResource0 = dxLayer_->skeletalMeshResources_["sk_test_0"];
    UINT vertexStride = sizeof(SkeletalVertex);
    UINT vertexOffset = 0;

    // Get the vertex shader
    VSResource vsResource = dxLayer_->vsResources_["SkeletalVertexShader"];

    // Get the pixel shader
    PSResource psResource = dxLayer_->psResources_["PixelShader"];

    // Set the vertex shader and update its constant buffers
    context->VSSetShader(vsResource.shader, nullptr, 0);

    // Set the pixel shader
    context->PSSetShader(psResource.shader, nullptr, 0);

    // Set the texture and texture sampler
    ID3D11ShaderResourceView* resources[2];
    resources[0] = dxLayer_->textureResources_["testTex"].texture;
    resources[1] = dxLayer_->textureResources_["testNorm"].texture;
    context->PSSetShaderResources(0, 2, resources);
    context->PSSetSamplers(0, 1, &dxLayer_->textureSampler_);

    // Set the vertex and index buffers to be drawn
    context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->IASetInputLayout(vsResource.layout);
    context->IASetVertexBuffers(0, 1, &meshResource0.vertexBuffer, &vertexStride, &vertexOffset);
    context->IASetIndexBuffer(meshResource0.indexBuffer, DXGI_FORMAT_R16_UINT, 0);
    context->DrawIndexed(meshResource0.indexCount, 0, 0);

    // Set the first parameter to 0 to disable VSync
    dxLayer_->swapChain_->Present(1, 0);
}