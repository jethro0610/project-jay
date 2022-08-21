#include "DXResources.h"
#include <assert.h>
#include "../Rendering/RenderTypes.h"

#include "../Logging/Logger.h"

#ifdef _DEBUG
#define D3D_FLAGS D3D11_CREATE_DEVICE_DEBUG
#define HRASSERT(hResult) assert(SUCCEEDED(hResult)); 
#else
#define D3D_FLAGS 0
#define HRASSERT(hResult) hResult;
#endif

using namespace DirectX;

DXResources::DXResources(HWND windowHandle, int width, int height) {
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
    CreateConstantBuffer(sizeof(PerObjectData), &perObjectCBuffer_);
    CreateConstantBuffer(sizeof(PerSkeletonData), &perSkeletonCBuffer_);
    ID3D11Buffer* cbuffers[2] = { perObjectCBuffer_, perSkeletonCBuffer_ };
    context_->VSSetConstantBuffers(0, 2, cbuffers);

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

    // Create the depth stencil buffer
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

    // Create vertex descriptions
    worldVertexDescription_[0] = skeletalVertexDescription_[0] = staticVertexDescription_[0] = {"POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0};
    worldVertexDescription_[1] = skeletalVertexDescription_[1] = staticVertexDescription_[1] = {"NORM", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(glm::vec3), D3D11_INPUT_PER_VERTEX_DATA, 0};
    skeletalVertexDescription_[2] = staticVertexDescription_[2] = {"TAN", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(glm::vec3) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0};
    skeletalVertexDescription_[3] = staticVertexDescription_[3] = {"BITAN", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(glm::vec3) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0};
    skeletalVertexDescription_[4] = staticVertexDescription_[4] = {"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(glm::vec3) * 4, D3D11_INPUT_PER_VERTEX_DATA, 0};
    skeletalVertexDescription_[5] = { "JOINTS", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, sizeof(glm::vec3) * 4 + sizeof(glm::vec2), D3D11_INPUT_PER_VERTEX_DATA, 0};
    skeletalVertexDescription_[6] = { "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(glm::vec3) * 4 + sizeof(glm::vec2) + sizeof(glm::vec4), D3D11_INPUT_PER_VERTEX_DATA, 0};

    CreateInputStructuredBufferAndView(
        sizeof(float), 
        DISTANCE_CACHE_SIZE* DISTANCE_CACHE_SIZE * DISTANCE_CACHE_SIZE, 
        &distanceCacheBuffer_, 
        &distanceCacheView_
    );
    CreateOutputStructuredBufferAndView(
        sizeof(vec3),
        WORLD_RESOLUTION * WORLD_RESOLUTION * WORLD_RESOLUTION,
        &computeWVertsBuffer_,
        &computeWVertsView_,
        &computeWVertsOutput_
    );
    ID3DBlob* computeWVertsBlob;
    HRASSERT(D3DReadFileToBlob(L"ComputeWorldVertices.cso", &computeWVertsBlob));
    HRASSERT(device_->CreateComputeShader(
        computeWVertsBlob->GetBufferPointer(),
        computeWVertsBlob->GetBufferSize(),
        nullptr,
        &computeWVertsShader_
    ));
    computeWVertsBlob->Release();

    InitWorldMeshes();
}

void DXResources::CreateConstantBuffer(int size, ID3D11Buffer** outBuffer) {
    // Create the per object cbuffer
    D3D11_BUFFER_DESC desc = {};
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = size;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    HRASSERT(device_->CreateBuffer(
        &desc,
        nullptr,
        outBuffer
    ));
}

void DXResources::CreateInputStructuredBufferAndView(int elementSize, int numberOfElements, ID3D11Buffer** outBuffer, ID3D11ShaderResourceView** outView) {
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    bufferDesc.ByteWidth = elementSize * numberOfElements;
    bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    bufferDesc.StructureByteStride = elementSize;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    HRASSERT(device_->CreateBuffer(&bufferDesc, nullptr, outBuffer));

    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
    viewDesc.BufferEx.FirstElement = 0;
    viewDesc.Format = DXGI_FORMAT_UNKNOWN;
    viewDesc.BufferEx.NumElements = numberOfElements;
    HRASSERT(device_->CreateShaderResourceView(*outBuffer, &viewDesc, outView));
}

void DXResources::CreateOutputStructuredBufferAndView(int elementSize, int numberOfElements, ID3D11Buffer** outBuffer, ID3D11UnorderedAccessView** outView, ID3D11Buffer** outStagingBuffer) {
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
    bufferDesc.ByteWidth = elementSize * numberOfElements;
    bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    bufferDesc.StructureByteStride = elementSize;
    HRASSERT(device_->CreateBuffer(&bufferDesc, nullptr, outBuffer));

    D3D11_UNORDERED_ACCESS_VIEW_DESC viewDesc = {};
    viewDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    viewDesc.Buffer.FirstElement = 0;
    viewDesc.Format = DXGI_FORMAT_UNKNOWN;
    viewDesc.Buffer.NumElements = numberOfElements;
    HRASSERT(device_->CreateUnorderedAccessView(*outBuffer, &viewDesc, outView));

    if (outStagingBuffer != nullptr) {
        bufferDesc.Usage = D3D11_USAGE_STAGING;
        bufferDesc.BindFlags = 0;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        HRASSERT(device_->CreateBuffer(&bufferDesc, 0, outStagingBuffer));
    }
}

void DXResources::WriteWorldMesh(ivec3 coordinates, const std::vector<WorldVertex>& vertices, const std::vector<uint16_t>& indices) {
    MeshResource worldMeshResource = worldMeshes_[coordinates.x][coordinates.y][coordinates.z];

    D3D11_MAPPED_SUBRESOURCE vertexResource;
    context_->Map(worldMeshResource.vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vertexResource);
    memcpy(vertexResource.pData, vertices.data(), sizeof(WorldVertex) * vertices.size());
    context_->Unmap(worldMeshResource.vertexBuffer, 0);

    D3D11_MAPPED_SUBRESOURCE indexResource;
    context_->Map(worldMeshResource.indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &indexResource);
    memcpy(indexResource.pData, indices.data(), sizeof(uint16_t) * indices.size());
    context_->Unmap(worldMeshResource.indexBuffer, 0);


    worldMeshes_[coordinates.x][coordinates.y][coordinates.z].vertexCount = vertices.size();
    worldMeshes_[coordinates.x][coordinates.y][coordinates.z].indexCount = indices.size();
}

DXResources::~DXResources() {
    device_->Release();
    swapChain_->Release();
    context_->Release();
    renderTarget_->Release();
    perObjectCBuffer_->Release();
    textureSampler_->Release();
    depthStencilBuffer_->Release();
}

void DXResources::LoadVertexShader(std::string shaderName, VertexShaderType shaderType) {
    assert(vertexShaders_.count(shaderName) == 0);
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

    ID3D11InputLayout* inputLayout = nullptr;
    switch (shaderType) {
    case VertexShaderType::STATIC:
        HRASSERT(device_->CreateInputLayout(
            staticVertexDescription_,
            ARRAYSIZE(staticVertexDescription_),
            vertexShaderBlob->GetBufferPointer(),
            vertexShaderBlob->GetBufferSize(),
            &inputLayout
        ));
        break;

    case VertexShaderType::SKELETAL:
        HRASSERT(device_->CreateInputLayout(
            skeletalVertexDescription_,
            ARRAYSIZE(skeletalVertexDescription_),
            vertexShaderBlob->GetBufferPointer(),
            vertexShaderBlob->GetBufferSize(),
            &inputLayout
        ));
        break;

    case VertexShaderType::WORLD:
        HRASSERT(device_->CreateInputLayout(
            worldVertexDescription_,
            ARRAYSIZE(worldVertexDescription_),
            vertexShaderBlob->GetBufferPointer(),
            vertexShaderBlob->GetBufferSize(),
            &inputLayout
        ));
        break;

    default:
        assert(true); // Should never default
        break;
    }
    vertexShaderBlob->Release();

    VSResource vsResource = {};
    vsResource.shader = vertexShader;
    vsResource.layout = inputLayout;
    vertexShaders_[shaderName] = vsResource;
}

void DXResources::LoadPixelShader(std::string shaderName) {
    assert(pixelShaders_.count(shaderName) == 0);
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
    pixelShaders_[shaderName] = psResource;
}

void DXResources::LoadModel(std::string modelName, bool skeletal) {
    RawModel rawModel((modelName + ".jmd").c_str(), skeletal);

    for (int i = 0; i < rawModel.meshes_.size(); i++) {
        LoadMesh(modelName, rawModel.meshes_[i], i, skeletal);
    }
}

void DXResources::LoadMesh(std::string modelName, RawMesh mesh, int meshIndex, bool skeletal) {
    std::string meshName = modelName + "_" + std::to_string(meshIndex);
    assert(staticMeshes_.count(meshName) == 0);

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

    if (skeletal)
        skeletalMeshes_[meshName] = meshResource;
    else
        staticMeshes_[meshName] = meshResource;
}

void DXResources::LoadTexture(std::string textureName) {
    std::string extensionName = textureName + ".png";
    std::wstring wString(extensionName.begin(), extensionName.end());
    TextureResource textureResource;

    // To disable mip generation, remove the context from the function
    HRASSERT(CreateWICTextureFromFile(device_, context_, wString.c_str(), nullptr, &textureResource.texture, 0));
    textures_[textureName] = textureResource;
}

void DXResources::InitWorldMeshes() {
    D3D11_BUFFER_DESC worldVBufferDesc = {};
    worldVBufferDesc.ByteWidth = sizeof(WorldVertex) * MAX_COORDINATE_VERTICES;
    worldVBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    worldVBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    worldVBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    D3D11_SUBRESOURCE_DATA worldVSrData = {};

    D3D11_BUFFER_DESC worldIBufferDesc = {};
    worldIBufferDesc.ByteWidth = sizeof(uint16_t) * MAX_COORDINATE_INDICES;
    worldIBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    worldIBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    worldIBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    D3D11_SUBRESOURCE_DATA worldISrData = {};
    worldISrData.pSysMem = &coordinateFillIndices_;

    for (int x = 0; x < MAX_X_COORDINATES; x++)
    for (int y = 0; y < MAX_Y_COORDINATES; y++)
    for (int z = 0; z < MAX_Z_COORDINATES; z++) {
        worldVSrData.pSysMem = &coordinateFillVertices_;
        HRASSERT(device_->CreateBuffer(
            &worldVBufferDesc,
            &worldVSrData,
            &worldMeshes_[x][y][z].vertexBuffer
        ));
        HRASSERT(device_->CreateBuffer(
            &worldIBufferDesc,
            &worldISrData,
            &worldMeshes_[x][y][z].indexBuffer
        ));
    }
}