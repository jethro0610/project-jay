#include "Renderer.h"

Renderer::Renderer(DirectXLayer* dxResources){
    dxLayer = dxResources;

    dxLayer->LoadVertexShader("VertexShader");
    dxLayer->LoadPixelShader("PixelShader");
    dxLayer->LoadModel("TestBuffer");
}

Renderer::~Renderer() {
    delete dxLayer;
}

void Renderer::Draw() {
    ID3D11DeviceContext* context = dxLayer->context_;
    ID3D11Buffer* vertexBuffer = dxLayer->vertexBuffers_["TestBuffer"];
    UINT vertexCount = dxLayer->vertexCounts_["TestBuffer"];
    UINT vertexStride = sizeof(ColorVertex);
    UINT vertexOffset = 0;

    ID3D11VertexShader* vertexShader = dxLayer->vertexShaders_["VertexShader"];
    ID3D11InputLayout* inputLayout = dxLayer->inputLayouts_["VertexShader"];

    ID3D11PixelShader* pixelShader = dxLayer->pixelShaders_["PixelShader"];

    float background_colour[4] = { 0x64 / 255.0f, 0x95 / 255.0f, 0xED / 255.0f, 1.0f };
    context->ClearRenderTargetView(dxLayer->renderTarget_, background_colour);

    D3D11_VIEWPORT viewport = {
        0.0f,
        0.0f,
        (float)dxLayer->renderWidth_,
        (float)dxLayer->renderHeight_,
        0.0f,
        1.0f 
    };
    context->RSSetViewports(1, &viewport);

    context->OMSetRenderTargets(1, &dxLayer->renderTarget_, nullptr);

    context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->IASetInputLayout(inputLayout);
    context->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexStride, &vertexOffset);

    context->VSSetShader(vertexShader, nullptr, 0);
    context->PSSetShader(pixelShader, nullptr, 0);
    context->Draw(vertexCount, 0);

    dxLayer->swapChain_->Present(1, 0);
}