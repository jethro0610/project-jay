#include "Renderer.h"

Renderer::Renderer(DirectXLayer* dxLayer){
    dxLayer_ = dxLayer;

    dxLayer_->LoadVertexShader("VertexShader");
    dxLayer_->LoadPixelShader("PixelShader");
    dxLayer_->LoadMesh("TestBuffer");
}

Renderer::~Renderer() {
   
}

void Renderer::PlatformRender() {
    ID3D11DeviceContext* context = dxLayer_->context_;

    float background_colour[4] = { 0x64 / 255.0f, 0x95 / 255.0f, 0xED / 255.0f, 1.0f };
    context->ClearRenderTargetView(dxLayer_->renderTarget_, background_colour);

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

    ID3D11Buffer* vertexBuffer = dxLayer_->vertexBuffers_["TestBuffer"];
    UINT vertexCount = dxLayer_->vertexCounts_["TestBuffer"];
    UINT vertexStride = sizeof(ColorVertex);
    UINT vertexOffset = 0;
    ID3D11Buffer* indexBuffer = dxLayer_->indexBuffers_["TestBuffer"];
    UINT indexCount = dxLayer_->indexCounts_["TestBuffer"];

    ID3D11VertexShader* vertexShader = dxLayer_->vertexShaders_["VertexShader"];
    ID3D11InputLayout* inputLayout = dxLayer_->inputLayouts_["VertexShader"];

    ID3D11PixelShader* pixelShader = dxLayer_->pixelShaders_["PixelShader"];

    context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->IASetInputLayout(inputLayout);
    context->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexStride, &vertexOffset);
    context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    context->VSSetShader(vertexShader, nullptr, 0);
    context->PSSetShader(pixelShader, nullptr, 0);
    context->DrawIndexed(indexCount, 0, 0);

    dxLayer_->swapChain_->Present(1, 0);
}