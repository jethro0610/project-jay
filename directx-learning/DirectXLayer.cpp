#include "DirectXLayer.h"
#include <assert.h>

#ifdef _DEBUG
#define D3D_FEATURE_LEVEL D3D11_CREATE_DEVICE_DEBUG
#define HRASSERT(hResult) assert(SUCCEEDED(hResult)); 
#else
#define D3D_FEATURE_LEVEL 0
#define HRASSERT(hResult) hResult;
#endif

DirectXLayer::DirectXLayer(HWND windowHandle, int width, int height) {
    renderWidth_ = width;
    renderHeight_ = height;

    DXGI_SWAP_CHAIN_DESC swapDesc = {};
    swapDesc.BufferDesc.Width = width;
    swapDesc.BufferDesc.Height = height;
    swapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapDesc.BufferDesc.RefreshRate.Denominator = 0;
    swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapDesc.SampleDesc.Count = 1;
    swapDesc.SampleDesc.Quality = 0;
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.BufferCount = 1;
    swapDesc.OutputWindow = windowHandle;
    swapDesc.Windowed = TRUE;
    swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapDesc.Flags = 0;

    HRASSERT(D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D_FEATURE_LEVEL,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &swapDesc,
        &swapChain_,
        &device_,
        nullptr,
        &context_
    ));

    // Create a render target to the back buffer
    ID3D11Resource* backBuffer = nullptr;
    swapChain_->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&backBuffer));
    HRASSERT(device_->CreateRenderTargetView(
        backBuffer,
        nullptr,
        &renderTarget_
    ));
    backBuffer->Release();
    
    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    flags |= D3DCOMPILE_DEBUG;
#endif
}

DirectXLayer::~DirectXLayer() {
    device_->Release();
    swapChain_->Release();
    context_->Release();
    renderTarget_->Release();
}

void DirectXLayer::LoadVertexShader(std::string fileName) {
    assert(vertexShaders_.count(fileName) == 0);
    std::string extentionName = fileName + ".cso";
    std::wstring wString(extentionName.begin(), extentionName.end());

    ID3DBlob* vertexShaderBlob;
    HRASSERT(D3DReadFileToBlob(wString.c_str(), &vertexShaderBlob));

    ID3D11VertexShader* vertexShader;
    HRASSERT(device_->CreateVertexShader(
        vertexShaderBlob->GetBufferPointer(),
        vertexShaderBlob->GetBufferSize(),
        nullptr,
        &vertexShader
    ));

    ID3D11InputLayout* inputLayout;
    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
        {"POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(glm::vec3), D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    HRASSERT(device_->CreateInputLayout(
        inputElementDesc,
        ARRAYSIZE(inputElementDesc),
        vertexShaderBlob->GetBufferPointer(),
        vertexShaderBlob->GetBufferSize(),
        &inputLayout
    ));

    vertexShaderBlobs_[fileName] = vertexShaderBlob;
    vertexShaders_[fileName] = vertexShader;
    inputLayouts_[fileName] = inputLayout;
}

void DirectXLayer::LoadPixelShader(std::string fileName) {
    assert(pixelShaders_.count(fileName) == 0);
    std::string extentionName = fileName + ".cso";
    std::wstring wString(extentionName.begin(), extentionName.end());

    ID3DBlob* pixelShaderBlob;
    HRASSERT(D3DReadFileToBlob(wString.c_str(), &pixelShaderBlob));

    ID3D11PixelShader* pixelShader;
    HRASSERT(device_->CreatePixelShader(
        pixelShaderBlob->GetBufferPointer(),
        pixelShaderBlob->GetBufferSize(),
        nullptr,
        &pixelShader
    ));

    pixelShaderBlobs_[fileName] = pixelShaderBlob;
    pixelShaders_[fileName] = pixelShader;
}

void DirectXLayer::LoadModel(std::string bufferName) {
    assert(vertexBuffers_.count(bufferName) == 0);

    ColorVertex vertexArray[] = {
        {{0.0f,  0.5f,  0.0f},      {1.0f,  1.0f,  0.0f}},
        {{0.5f,  -0.755f,  0.0f},   {0.0f,  1.0f,  0.0f}},
        {{-0.5f,  -0.5f,  0.0f},    {0.0f,  0.0f,  1.0f}}
    };

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = sizeof(vertexArray);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA srData = {};
    srData.pSysMem = vertexArray;

    ID3D11Buffer* vertexBuffer;
    HRASSERT(device_->CreateBuffer(
        &bufferDesc,
        &srData,
        &vertexBuffer
    ));

    vertexBuffers_[bufferName] = vertexBuffer;
    vertexCounts_[bufferName] = 3;
}

/*
void Rendering::Draw() {
    float background_colour[4] = { 0x64 / 255.0f, 0x95 / 255.0f, 0xED / 255.0f, 1.0f };
    context_->ClearRenderTargetView(renderTarget_, background_colour);

    D3D11_VIEWPORT viewport = {
      0.0f,
      0.0f,
      (float)renderWidth_,
      (float)renderHeight_,
      0.0f,
      1.0f };
    context_->RSSetViewports(1, &viewport);

    context_->OMSetRenderTargets(1, &renderTarget_, nullptr);

    context_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context_->IASetInputLayout(inputLayout_);
    context_->IASetVertexBuffers(0, 1, &vertexBuffer_, &vertexStride_, &vertexOffset_);

    context_->VSSetShader(vertexShader_, nullptr, 0);
    context_->PSSetShader(pixelShader_, nullptr, 0);
    context_->Draw(vertexCount_, 0);

    swapChain_->Present(1, 0);
}
*/