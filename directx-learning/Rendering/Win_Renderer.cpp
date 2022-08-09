#include "Renderer.h"

Renderer::Renderer(DirectXLayer* dxLayer){
    dxLayer_ = dxLayer;

    dxLayer_->LoadVertexShader("VertexShader");
    dxLayer_->LoadPixelShader("PixelShader");
    dxLayer_->LoadMesh("TestBuffer");

    width_ = dxLayer_->renderWidth_;
    height_ = dxLayer_->renderHeight_;

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
        (float)dxLayer_->renderWidth_,
        (float)dxLayer_->renderHeight_,
        0.0f,
        1.0f 
    };
    context->RSSetViewports(1, &viewport);
    context->OMSetRenderTargets(1, &dxLayer_->renderTarget_, nullptr);

    // Get the vertex and index variables
    ID3D11Buffer* vertexBuffer = dxLayer_->vertexBuffers_["TestBuffer"];
    UINT vertexCount = dxLayer_->vertexCounts_["TestBuffer"];
    UINT vertexStride = sizeof(ColorVertex);
    UINT vertexOffset = 0;
    ID3D11Buffer* indexBuffer = dxLayer_->indexBuffers_["TestBuffer"];
    UINT indexCount = dxLayer_->indexCounts_["TestBuffer"];

    // Get the vertex shader
    ID3D11VertexShader* vertexShader = dxLayer_->vertexShaders_["VertexShader"];
    ID3D11InputLayout* inputLayout = dxLayer_->inputLayouts_["VertexShader"];

    // Get the pixel shader
    ID3D11PixelShader* pixelShader = dxLayer_->pixelShaders_["PixelShader"];

    // Set the vertex and index buffers to be drawn
    context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->IASetInputLayout(inputLayout);
    context->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexStride, &vertexOffset);
    context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the vertex shader and update its constant buffers
    context->VSSetShader(vertexShader, nullptr, 0);
    context->VSSetConstantBuffers(0, 1, &dxLayer_->perObjectCBuffer_);

    // Set the pixel shader
    context->PSSetShader(pixelShader, nullptr, 0);

    // Draw the mesh
    context->DrawIndexed(indexCount, 0, 0);

    // Present the frame
    dxLayer_->swapChain_->Present(1, 0);
}