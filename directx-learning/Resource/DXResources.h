#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <WICTextureLoader.h>
#include <unordered_map>
#include <string>
#include "../Resource/RawModel.h"
#include "../Rendering/RenderTypes.h"

// Keep shader and layout together since they're accessed at the same time
// Redundant resource structs with single elements are used for consistency with the maps
struct VSResource {
    ID3D11VertexShader* shader;
    ID3D11InputLayout* layout;
};

struct PSResource {
    ID3D11PixelShader* shader;
};

struct MeshResource {
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;
    UINT vertexCount;
    UINT indexCount;
};

struct TextureResource {
    ID3D11ShaderResourceView* texture;
};

enum VertexShaderType {
    STATIC,
    SKELETAL,
    WORLD
};

class DXResources {
public:
    DXResources(HWND windowHandle, int width, int height);
    ~DXResources();

    std::unordered_map<std::string, VSResource> vertexShaders_;
    std::unordered_map<std::string, PSResource> pixelShaders_;
    std::unordered_map<std::string, MeshResource> staticMeshes_;
    std::unordered_map<std::string, MeshResource> skeletalMeshes_;
    std::unordered_map<std::string, TextureResource> textures_;

    ID3D11Device* device_;
    IDXGISwapChain* swapChain_;
    ID3D11DeviceContext* context_;
    ID3D11RenderTargetView* renderTarget_;
    ID3D11Buffer* perObjectCBuffer_;
    ID3D11Buffer* perSkeletonCBuffer_;
    ID3D11SamplerState* textureSampler_;
    ID3D11DepthStencilView* depthStencilBuffer_;

    D3D11_INPUT_ELEMENT_DESC worldVertexDescription_[2];
    D3D11_INPUT_ELEMENT_DESC staticVertexDescription_[5];
    D3D11_INPUT_ELEMENT_DESC skeletalVertexDescription_[7];

    ID3D11Buffer* temp_worldVertexBuffer_;
    int temp_worldVertexCount_;
    ID3D11Buffer* temp_worldIndexBuffer_;
    int temp_worldIndexCount_;

    void Temp_UpdateWorld(const std::vector<WorldVertex>& vertices, const std::vector<uint16_t>& indices);

    int width_;
    int height_;

    void LoadVertexShader(std::string shaderName, VertexShaderType shaderType = VertexShaderType::STATIC);
    void LoadPixelShader(std::string shaderName);
    void LoadModel(std::string modelName, bool skeletal = false);
    void LoadTexture(std::string textureName);

private:
    void LoadMesh(std::string modelName, RawMesh mesh, int meshIndex, bool skeletal);
};