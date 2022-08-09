#include "Renderer.h"

Renderer::Renderer(DirectXLayer* dxLayer){
    dxLayer_ = dxLayer;

    dxLayer_->LoadVertexShader("VertexShader");
    dxLayer_->LoadPixelShader("PixelShader");
    dxLayer_->LoadMesh("TestMesh");

    width_ = dxLayer->width_;
    height_ = dxLayer->height_;

    Init();
}

Renderer::~Renderer() {
   
}

void Renderer::PlatformRender() {
    ID3D11DeviceContext* context = dxLayer_->context_;

    PerObjectData objectData = {};
    objectData.worldViewProj = GetWorldViewProjection();

    // Update the per frame constant buffer
    context->UpdateSubresource(dxLayer_->perObjectCBuffer_, 0, nullptr, &objectData, 0, 0);

    // Clear the render target with background color
    float background_colour[4] = { 0x64 / 255.0f, 0x95 / 255.0f, 0xED / 255.0f, 1.0f };
    context->ClearRenderTargetView(dxLayer_->renderTarget_, background_colour);

    // Update the viewport
    D3D11_VIEWPORT viewport = {
        0.0f,
        0.0f,
        (float)width_,
        (float)height_,
        0.0f,
        1.0f 
    };
    context->RSSetViewports(1, &viewport);
    context->OMSetRenderTargets(1, &dxLayer_->renderTarget_, nullptr);

    // Get the vertex and index variables
    MeshResource meshResource = dxLayer_->meshResources_["TestMesh"];
    UINT vertexStride = sizeof(Vertex);
    UINT vertexOffset = 0;

    // Get the vertex shader
    VSResource vsResource = dxLayer_->vsResources_["VertexShader"];

    // Get the pixel shader
    PSResource psResource = dxLayer_->psResources_["PixelShader"];

    // Set the vertex and index buffers to be drawn
    context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->IASetInputLayout(vsResource.layout);
    context->IASetVertexBuffers(0, 1, &meshResource.vertexBuffer, &vertexStride, &vertexOffset);
    context->IASetIndexBuffer(meshResource.indexBuffer, DXGI_FORMAT_R16_UINT, 0);

    // Set the vertex shader and update its constant buffers
    context->VSSetShader(vsResource.shader, nullptr, 0);
    context->VSSetConstantBuffers(0, 1, &dxLayer_->perObjectCBuffer_);

    // Set the pixel shader
    context->PSSetShader(psResource.shader, nullptr, 0);

    // Draw the mesh
    context->DrawIndexed(meshResource.indexCount, 0, 0);

    // Present the frame
    dxLayer_->swapChain_->Present(1, 0);
}