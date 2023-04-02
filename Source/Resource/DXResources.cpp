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

    // Create the post process render target texture
    LoadPixelShader("PostProcess");
    D3D11_TEXTURE2D_DESC rtDesc = {};
    rtDesc.Width = width_;
    rtDesc.Height = height_;
    rtDesc.MipLevels = 1;
    rtDesc.ArraySize = 1;
    rtDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    rtDesc.SampleDesc.Count = 1;
    rtDesc.Usage = D3D11_USAGE_DEFAULT;
    rtDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    rtDesc.CPUAccessFlags = 0;
    rtDesc.MiscFlags = 0;
    device_->CreateTexture2D(&rtDesc, nullptr, &pRenderTexture_);
    D3D11_RENDER_TARGET_VIEW_DESC rtViewDesc = {};
    rtViewDesc.Format = rtDesc.Format;
    rtViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtViewDesc.Texture2D.MipSlice = 0;
    device_->CreateRenderTargetView(pRenderTexture_, &rtViewDesc, &pRenderTarget_);
    D3D11_SHADER_RESOURCE_VIEW_DESC rtResourceViewDesc = {};
    rtResourceViewDesc.Format = rtDesc.Format;
    rtResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    rtResourceViewDesc.Texture2D.MostDetailedMip = 0;
    rtResourceViewDesc.Texture2D.MipLevels= 1;
    device_->CreateShaderResourceView(pRenderTexture_, &rtResourceViewDesc, &pRenderTextureResource_);
    
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
    
    // Set the viewport 
    D3D11_VIEWPORT viewport = {
        0.0f,
        0.0f,
        (float)width_,
        (float)height_,
        0.0f,
        1.0f
    };
    context_->RSSetViewports(1, &viewport);

    // Create the per object cbuffer
    CreateConstantBuffer(sizeof(PerFrameData), &perFrameCBuffer_);
    CreateConstantBuffer(sizeof(PerObjectData), &perObjectCBuffer_);
    CreateConstantBuffer(sizeof(PerSkeletonData), &perSkeletonCBuffer_);
    CreateConstantBuffer(sizeof(vec4), &perChunkCBuffer_); 

    ID3D11Buffer* cbuffers[3] = { perFrameCBuffer_, perObjectCBuffer_, perSkeletonCBuffer_ };
    context_->VSSetConstantBuffers(0, 3, cbuffers);
    context_->PSSetConstantBuffers(0, 3, cbuffers); // TODO: Seperate the buffers to only whats needed for PS and VS
    context_->CSSetConstantBuffers(0, 1, &perChunkCBuffer_);

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

    // Create vertex descriptions
    worldVertexDescription_[0] = 
    skeletalVertexDescription_[0] = 
    staticVertexDescription_[0] = 
    instancedVertexDescription_[0] = {
        "POS", 
        0, 
        DXGI_FORMAT_R32G32B32_FLOAT, 
        0,
        0, 
        D3D11_INPUT_PER_VERTEX_DATA,
        0
    };
    worldVertexDescription_[1] =
    skeletalVertexDescription_[1] = 
    staticVertexDescription_[1] = 
    instancedVertexDescription_[1] = {
        "NORM", 
        0, 
        DXGI_FORMAT_R32G32B32_FLOAT, 
        0, 
        sizeof(glm::vec3), 
        D3D11_INPUT_PER_VERTEX_DATA, 
        0
    };
    skeletalVertexDescription_[2] = 
    staticVertexDescription_[2] =
    instancedVertexDescription_[2] = {
        "TAN", 
        0, 
        DXGI_FORMAT_R32G32B32_FLOAT, 
        0, 
        sizeof(glm::vec3) * 2, 
        D3D11_INPUT_PER_VERTEX_DATA, 
        0
    };
    skeletalVertexDescription_[3] = 
    staticVertexDescription_[3] = 
    instancedVertexDescription_[3] = {
        "BITAN", 
        0, 
        DXGI_FORMAT_R32G32B32_FLOAT, 
        0, 
        sizeof(glm::vec3) * 3, 
        D3D11_INPUT_PER_VERTEX_DATA, 
        0
    };
    skeletalVertexDescription_[4] = 
    staticVertexDescription_[4] = 
    instancedVertexDescription_[4] = {
        "UV", 
        0, 
        DXGI_FORMAT_R32G32_FLOAT, 
        0, 
        sizeof(glm::vec3) * 4, 
        D3D11_INPUT_PER_VERTEX_DATA, 
        0
    };
    skeletalVertexDescription_[5] = { 
        "JOINTS", 
        0, 
        DXGI_FORMAT_R32G32B32A32_SINT, 
        0, 
        sizeof(glm::vec3) * 4 + sizeof(glm::vec2), 
        D3D11_INPUT_PER_VERTEX_DATA, 
        0
    };
    skeletalVertexDescription_[6] = { 
        "WEIGHTS", 
        0, 
        DXGI_FORMAT_R32G32B32A32_FLOAT, 
        0, 
        sizeof(glm::vec3) * 4 + sizeof(glm::vec2) + sizeof(glm::vec4), 
        D3D11_INPUT_PER_VERTEX_DATA, 
        0
    };
    instancedVertexDescription_[5] = {
        "INST_POS",
        0,
        DXGI_FORMAT_R32G32B32_FLOAT,
        1,
        0,
        D3D11_INPUT_PER_INSTANCE_DATA,
        1
    };
    LoadVertexShader("ScreenQuad");
    
    // Setup the world vertex compute shader
    CreateStructuredBufferAndView(
        sizeof(WorldVertex),
        MAX_CHUNK_VERTICES,
        &computeWVertsBuffer_,
        &computeWVertsView_,
        false,
        nullptr
    );
    CreateStructuredBufferAndView(
        sizeof(int) * 8,
        MAX_CHUNK_VERTICES,
        &computeWValidBuffer_,
        &computeWValidView_,
        false,
        nullptr
    );
    CreateStructuredBufferAndView(
        sizeof(uint) * 6,
        MAX_CHUNK_QUADS,
        &computeWQuadsBuffer_,
        &computeWQuadsView_,
        true,
        nullptr
    );
    CreateStructuredBufferAndView(
        sizeof(uint),
        1,
        &computeWCountBuffer_,
        &computeWCountView_,
        false,
        &computeWCountOutput_
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

    ID3DBlob* computeWQuadsBlob;
    HRASSERT(D3DReadFileToBlob(L"ComputeWorldQuads.cso", &computeWQuadsBlob));
    HRASSERT(device_->CreateComputeShader(
        computeWQuadsBlob->GetBufferPointer(),
        computeWQuadsBlob->GetBufferSize(),
        nullptr,
        &computeWQuadsShader_
    ));
    computeWQuadsBlob->Release();


    D3D11_BLEND_DESC noBlendDesc = {};
    noBlendDesc.RenderTarget[0].BlendEnable = FALSE;
    noBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    HRASSERT(device_->CreateBlendState(&noBlendDesc, &noBlendState_));

    D3D11_BLEND_DESC alphaBlendDesc = {};
    alphaBlendDesc.RenderTarget[0].BlendEnable = TRUE;
    alphaBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    alphaBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    alphaBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    alphaBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
    alphaBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
    alphaBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    alphaBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    HRASSERT(device_->CreateBlendState(&alphaBlendDesc, &alphaBlendState_));

    InitWorldMeshes();
    InitSpreadBuffers();
    InitText();
    InitNoiseTexture();
}

void DXResources::UpdateBuffer(ID3D11Buffer* buffer, void* data, int size) {
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    context_->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, data, size);
    context_->Unmap(buffer, 0);
}

void DXResources::CreateConstantBuffer(int size, ID3D11Buffer** outBuffer) {
    // Create the per object cbuffer
    D3D11_BUFFER_DESC desc = {};
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.ByteWidth = size;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.MiscFlags = 0;
    HRASSERT(device_->CreateBuffer(
        &desc,
        nullptr,
        outBuffer
    ));
}

void DXResources::CreateStructuredBufferAndView(
    int elementSize, 
    int numberOfElements, 
    ID3D11Buffer** outBuffer, 
    ID3D11UnorderedAccessView** outView, 
    bool append,
    ID3D11Buffer** outStagingBuffer
) {
    std::string str = "Element size: " + std::to_string(elementSize) + '\n';
    std::wstring wStr = std::wstring(str.begin(), str.end());
    LPCWSTR outStr = wStr.c_str();
    OutputDebugStringW(outStr);

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
    if (append)
        viewDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_APPEND;
    else
        viewDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_COUNTER;
    HRASSERT(device_->CreateUnorderedAccessView(*outBuffer, &viewDesc, outView));

    if (outStagingBuffer != nullptr) {
        bufferDesc.Usage = D3D11_USAGE_STAGING;
        bufferDesc.BindFlags = 0;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        HRASSERT(device_->CreateBuffer(&bufferDesc, 0, outStagingBuffer));
    }
}

void DXResources::WriteWorldMesh(ivec3 chunk, const std::vector<WorldVertex>& vertices, const std::vector<uint16_t>& indices) {
    ivec3 normalizedChunk = chunk;
    normalizedChunk.x += MAX_X_CHUNKS / 2;
    normalizedChunk.y += MAX_Y_CHUNKS / 2;
    normalizedChunk.z += MAX_Z_CHUNKS / 2;

    DXMesh worldMesh = worldMeshes_[normalizedChunk.x][normalizedChunk.y][normalizedChunk.z];

    D3D11_MAPPED_SUBRESOURCE vertexResource;
    context_->Map(worldMesh.vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vertexResource);
    memcpy(vertexResource.pData, vertices.data(), sizeof(WorldVertex) * vertices.size());
    context_->Unmap(worldMesh.vertexBuffer, 0);

    D3D11_MAPPED_SUBRESOURCE indexResource;
    context_->Map(worldMesh.indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &indexResource);
    memcpy(indexResource.pData, indices.data(), sizeof(uint16_t) * indices.size());
    context_->Unmap(worldMesh.indexBuffer, 0);

    worldMeshes_[normalizedChunk.x][normalizedChunk.y][normalizedChunk.z].vertexCount = vertices.size();
    worldMeshes_[normalizedChunk.x][normalizedChunk.y][normalizedChunk.z].indexCount = indices.size();
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

    case VertexShaderType::INSTANCED:
        HRASSERT(device_->CreateInputLayout(
            instancedVertexDescription_,
            ARRAYSIZE(instancedVertexDescription_),
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

    VSLayout vsLayout = {};
    vsLayout.shader = vertexShader;
    vsLayout.layout = inputLayout;
    vertexShaders_[shaderName] = vsLayout;
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

    pixelShaders_[shaderName] = pixelShader;
}

void DXResources::LoadRawModel(RawModel& rawModel, std::string modelName, bool skeletal) {
    for (int i = 0; i < rawModel.meshes_.size(); i++) {
        LoadMesh(modelName, rawModel.meshes_[i], i, skeletal);
    }
}

void DXResources::LoadMesh(std::string modelName, RawMesh mesh, int meshIndex, bool skeletal) {
    std::string meshName = modelName + "_" + std::to_string(meshIndex);
    if (skeletal)
        assert(skeletalMeshes_.count(meshName) == 0);
    else
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

    DXMesh dxMesh = {};
    dxMesh.vertexBuffer = vertexBuffer;
    dxMesh.vertexCount = mesh.vertexCount_;
    dxMesh.indexBuffer = indexBuffer;
    dxMesh.indexCount = mesh.indexCount_;

    if (skeletal)
        skeletalMeshes_[meshName] = dxMesh;
    else
        staticMeshes_[meshName] = dxMesh;
}

void DXResources::LoadTexture(std::string textureName) {
    std::string extensionName = textureName + ".png";
    std::wstring wString(extensionName.begin(), extensionName.end());
    ID3D11ShaderResourceView* texture;
    // To disable mip generation, remove the context from the function
    HRASSERT(CreateWICTextureFromFile(device_, context_, wString.c_str(), nullptr, &texture, 0));
    textures_[textureName] = texture;
}

void DXResources::InitWorldMeshes() {
    D3D11_BUFFER_DESC worldVBufferDesc = {};
    worldVBufferDesc.ByteWidth = sizeof(WorldVertex) * MAX_CHUNK_VERTICES;
    worldVBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    worldVBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    worldVBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    D3D11_SUBRESOURCE_DATA worldVSrData = {};
    worldVSrData.pSysMem = &chunkFillVertices_;

    D3D11_BUFFER_DESC worldIBufferDesc = {};
    worldIBufferDesc.ByteWidth = sizeof(uint) * 6 * MAX_CHUNK_QUADS;
    worldIBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    worldIBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    worldIBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    D3D11_SUBRESOURCE_DATA worldISrData = {};
    worldISrData.pSysMem = &chunkFillIndices_;

    for (int x = 0; x < MAX_X_CHUNKS; x++)
    for (int y = 0; y < MAX_Y_CHUNKS; y++)
    for (int z = 0; z < MAX_Z_CHUNKS; z++) {
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

void DXResources::InitSpreadBuffers() {
    D3D11_BUFFER_DESC desc = {};
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.ByteWidth = sizeof(InstanceData) * MAX_SPREAD; 
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.MiscFlags = 0;

    for (int x = 0; x < MAX_X_CHUNKS; x++)
    for (int z = 0; z < MAX_Z_CHUNKS; z++) {
        HRASSERT(device_->CreateBuffer(&desc, nullptr, &spreadBuffers_[x][z]));
    }
}

void DXResources::InitText() {
    D3D11_BUFFER_DESC textDesc = {};
    textDesc.Usage = D3D11_USAGE_DYNAMIC;
    textDesc.ByteWidth = sizeof(TextData) * MAX_LINES * CHARS_PER_LINE; 
    textDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    textDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    textDesc.MiscFlags = 0;
    HRASSERT(device_->CreateBuffer(&textDesc, nullptr, &textBuffer_));
    ID3DBlob* vertexShaderBlob;
    HRASSERT(D3DReadFileToBlob(L"TextVS.cso", &vertexShaderBlob));
    HRASSERT(device_->CreateVertexShader(
        vertexShaderBlob->GetBufferPointer(),
        vertexShaderBlob->GetBufferSize(),
        nullptr,
        &textVS_
    ));

    D3D11_INPUT_ELEMENT_DESC textVertexDescription[] = {{"CHAR_INDEX", 0, DXGI_FORMAT_R32_UINT, 0, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1}};
    HRASSERT(device_->CreateInputLayout(
        textVertexDescription,
        ARRAYSIZE(textVertexDescription),
        vertexShaderBlob->GetBufferPointer(),
        vertexShaderBlob->GetBufferSize(),
        &textVSLayout_
    ));

    ID3DBlob* pixelShaderBlob;
    HRASSERT(D3DReadFileToBlob(L"TextPS.cso", &pixelShaderBlob));

    HRASSERT(device_->CreatePixelShader(
        pixelShaderBlob->GetBufferPointer(),
        pixelShaderBlob->GetBufferSize(),
        nullptr,
        &textPS_
    ));
    HRASSERT(CreateWICTextureFromFile(device_, context_, L"font.bmp", nullptr, &textTexture_, 0));
}

void DXResources::InitNoiseTexture() {
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = NOISE_SIZE;
    desc.Height = NOISE_SIZE;
    desc.MipLevels = desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R32_FLOAT;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.MiscFlags = 0;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;

    device_->CreateTexture2D(&desc, NULL, &noiseTexture_);

    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
    viewDesc.Format = desc.Format;
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    viewDesc.Texture2D.MostDetailedMip = 0;
    viewDesc.Texture2D.MipLevels= 1;
    device_->CreateShaderResourceView(noiseTexture_, &viewDesc, &noiseTextureSRV_);
}
