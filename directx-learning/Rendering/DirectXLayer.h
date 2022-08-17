#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <WICTextureLoader.h>
#include <unordered_map>
#include <string>
#include "../Skeleton.h"
#include "../RawModel.h"

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

class DirectXLayer {
public:
    DirectXLayer(HWND windowHandle, int width, int height);
    ~DirectXLayer();

    std::unordered_map<std::string, VSResource> vsResources_;
    std::unordered_map<std::string, PSResource> psResources_;
    std::unordered_map<std::string, MeshResource> staticMeshResources_;
    std::unordered_map<std::string, MeshResource> skeletalMeshResources_;
    std::unordered_map<std::string, Skeleton> skeletons_;
    std::unordered_map<std::string, TextureResource> textureResources_;

    ID3D11Device* device_;
    IDXGISwapChain* swapChain_;
    ID3D11DeviceContext* context_;
    ID3D11RenderTargetView* renderTarget_;
    ID3D11Buffer* perObjectCBuffer_;
    ID3D11SamplerState* textureSampler_;
    ID3D11DepthStencilView* depthStencilBuffer_;

    D3D11_INPUT_ELEMENT_DESC staticVertexDescription_[5];
    D3D11_INPUT_ELEMENT_DESC skeletalVertexDescription_[7];

    int width_;
    int height_;

    void LoadVertexShader(std::string shaderName, bool skeletal = false);
    void LoadPixelShader(std::string shaderName);
    void LoadModel(std::string modelName, bool skeletal = false);
    void LoadTexture(std::string textureName);

private:
    void LoadMesh(std::string modelName, RawMesh mesh, int meshIndex, bool skeletal);
};