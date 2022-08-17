#include "DirectXLayer.h"
#include <assert.h>
#include "RenderTypes.h"

#include "../Logger.h"

#ifdef _DEBUG
#define D3D_FLAGS D3D11_CREATE_DEVICE_DEBUG
#define HRASSERT(hResult) assert(SUCCEEDED(hResult)); 
#else
#define D3D_FLAGS 0
#define HRASSERT(hResult) hResult;
#endif

using namespace DirectX;

DirectXLayer::DirectXLayer(HWND windowHandle, int width, int height) {
    width_ = width;
    height_ = height;

    // Make the swap chain
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
        D3D_FLAGS,
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

    // Create the per object cbuffer
    D3D11_BUFFER_DESC perObjectDesc = {};
    perObjectDesc.Usage = D3D11_USAGE_DEFAULT;
    perObjectDesc.ByteWidth = sizeof(PerObjectData);
    perObjectDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    perObjectDesc.CPUAccessFlags = 0;
    perObjectDesc.MiscFlags = 0;
    HRASSERT(device_->CreateBuffer(
        &perObjectDesc,
        nullptr,
        &perObjectCBuffer_
    ));

    // Create texture sampler
    D3D11_SAMPLER_DESC tSampDesc = {};
    tSampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    tSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    tSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    tSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    tSampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    tSampDesc.MinLOD = 0;
    tSampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    HRASSERT(device_->CreateSamplerState(&tSampDesc, &textureSampler_));

    D3D11_TEXTURE2D_DESC dsDesc;
    dsDesc.Width = width;
    dsDesc.Height = height;
    dsDesc.MipLevels = 1;
    dsDesc.ArraySize = 1;
    dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsDesc.SampleDesc.Count = 1;
    dsDesc.SampleDesc.Quality = 0;
    dsDesc.Usage = D3D11_USAGE_DEFAULT;
    dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    dsDesc.CPUAccessFlags = 0;
    dsDesc.MiscFlags = 0;
    ID3D11Texture2D* depthStencilResource;
    HRASSERT(device_->CreateTexture2D(&dsDesc, nullptr, &depthStencilResource));
    HRASSERT(device_->CreateDepthStencilView(depthStencilResource, nullptr, &depthStencilBuffer_));
    depthStencilResource->Release();

    // Set the viewport and output merger
    D3D11_VIEWPORT viewport = {
        0.0f,
        0.0f,
        (float)width_,
        (float)height_,
        0.0f,
        1.0f
    };
    context_->RSSetViewports(1, &viewport);
    context_->OMSetRenderTargets(1, &renderTarget_, depthStencilBuffer_);
}

DirectXLayer::~DirectXLayer() {
    device_->Release();
    swapChain_->Release();
    context_->Release();
    renderTarget_->Release();
    perObjectCBuffer_->Release();
    textureSampler_->Release();
    depthStencilBuffer_->Release();
}

void DirectXLayer::LoadVertexShader(std::string shaderName) {
    assert(vsResources_.count(shaderName) == 0);
    std::string extensionName = shaderName + ".cso";
    std::wstring wString(extensionName.begin(), extensionName.end());

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
        {"NORM", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(glm::vec3), D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TAN", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(glm::vec3) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"BITAN", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(glm::vec3) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(glm::vec3) * 4, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    HRASSERT(device_->CreateInputLayout(
        inputElementDesc,
        ARRAYSIZE(inputElementDesc),
        vertexShaderBlob->GetBufferPointer(),
        vertexShaderBlob->GetBufferSize(),
        &inputLayout
    ));
    vertexShaderBlob->Release();

    VSResource vsResource = {};
    vsResource.shader = vertexShader;
    vsResource.layout = inputLayout;
    vsResources_[shaderName] = vsResource;
}

void DirectXLayer::LoadPixelShader(std::string shaderName) {
    assert(psResources_.count(shaderName) == 0);
    std::string extensionName = shaderName + ".cso";
    std::wstring wString(extensionName.begin(), extensionName.end());

    ID3DBlob* pixelShaderBlob;
    HRASSERT(D3DReadFileToBlob(wString.c_str(), &pixelShaderBlob));

    ID3D11PixelShader* pixelShader;
    HRASSERT(device_->CreatePixelShader(
        pixelShaderBlob->GetBufferPointer(),
        pixelShaderBlob->GetBufferSize(),
        nullptr,
        &pixelShader
    ));

    PSResource psResource = {};
    psResource.shader = pixelShader;
    psResources_[shaderName] = psResource;
}

void DirectXLayer::LoadModel(std::string modelName) {
    RawModel rawModel((modelName + ".jmd").c_str());

    for (int i = 0; i < rawModel.meshes_.size(); i++) {
        LoadMesh(modelName, rawModel.meshes_[i], i);
    }
}

void DirectXLayer::LoadMesh(std::string modelName, RawMesh mesh, int meshIndex) {
    std::string meshName = modelName + "_" + std::to_string(meshIndex);
    assert(staticMeshResources_.count(meshName) == 0);

    D3D11_BUFFER_DESC vBufferDesc = {};
    vBufferDesc.ByteWidth = mesh.GetVertexByteWidth();
    vBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA vSrData = {};
    vSrData.pSysMem = mesh.vertexBuffer_;

    ID3D11Buffer* vertexBuffer;
    HRASSERT(device_->CreateBuffer(
        &vBufferDesc,
        &vSrData,
        &vertexBuffer
    ));

    D3D11_BUFFER_DESC iBufferDesc = {};
    iBufferDesc.ByteWidth = mesh.GetIndexByteWidth();
    iBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    iBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    iBufferDesc.CPUAccessFlags = 0;
    iBufferDesc.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA iSrData = {};
    iSrData.pSysMem = mesh.indexBuffer_;

    ID3D11Buffer* indexBuffer;
    HRASSERT(device_->CreateBuffer(
        &iBufferDesc,
        &iSrData,
        &indexBuffer
    ));

    MeshResource meshResource = {};
    meshResource.vertexBuffer = vertexBuffer;
    meshResource.vertexCount = mesh.vertexCount_;
    meshResource.indexBuffer = indexBuffer;
    meshResource.indexCount = mesh.indexCount_;

    staticMeshResources_[meshName] = meshResource;
}

void DirectXLayer::LoadTexture(std::string textureName) {
    std::string extensionName = textureName + ".png";
    std::wstring wString(extensionName.begin(), extensionName.end());
    TextureResource textureResource;

    // To disable mip generation, remove the context from the function
    HRASSERT(CreateWICTextureFromFile(device_, context_, wString.c_str(), nullptr, &textureResource.texture, 0));
    textureResources_[textureName] = textureResource;
}