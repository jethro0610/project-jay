#include "Renderer.h"

Renderer::Renderer(DirectXLayer* dxLayer){
    dxLayer_ = dxLayer;

    dxLayer_->LoadVertexShader("VertexShader");
    dxLayer_->LoadPixelShader("PixelShader");
    dxLayer_->LoadModel("m_cube");

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
    MeshResource meshResource0 = dxLayer_->meshResources_["m_cube_0"];
    MeshResource meshResource1 = dxLayer_->meshResources_["m_cube_1"];

    UINT vertexStride = sizeof(Vertex);
    UINT vertexOffset = 0;

    // Get the vertex shader
    VSResource vsResource = dxLayer_->vsResources_["VertexShader"];

    // Get the pixel shader
    PSResource psResource = dxLayer_->psResources_["PixelShader"];

    // Set the vertex shader and update its constant buffers
    context->VSSetShader(vsResource.shader, nullptr, 0);
    context->VSSetConstantBuffers(0, 1, &dxLayer_->perObjectCBuffer_);

    // Set the pixel shader
    context->PSSetShader(psResource.shader, nullptr, 0);


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

    // Present the frame
    dxLayer_->swapChain_->Present(1, 0);
}