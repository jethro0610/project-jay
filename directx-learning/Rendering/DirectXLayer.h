#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <WICTextureLoader.h>
#include <unordered_map>
#include <string>
#include "../RawModel.h"

struct VSResource {
    ID3D11VertexShader* shader;
    ID3D11InputLayout* layout;
    ID3DBlob* blob;
};

struct PSResource {
    ID3D11PixelShader* shader;
    ID3DBlob* blob;
};

struct MeshResource {
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;
    UINT vertexCount;
    UINT indexCount;
};

struct TextureResource {
    ID3D11Resource* texture;
    ID3D11ShaderResourceView* view;
};

class DirectXLayer {
public:
    DirectXLayer(HWND windowHandle, int width, int height);
    ~DirectXLayer();

    std::unordered_map<std::string, VSResource> vsResources_;
    std::unordered_map<std::string, PSResource> psResources_;
    std::unordered_map<std::string, MeshResource> meshResources_;
    std::unordered_map<std::string, TextureResource> textureResources_;

    ID3D11Device* device_;
    IDXGISwapChain* swapChain_;
    ID3D11DeviceContext* context_;
    ID3D11RenderTargetView* renderTarget_;
    ID3D11Buffer* perObjectCBuffer_;
    ID3D11SamplerState* textureSampler_;

    ID3D11DepthStencilView* depthStencilView_;
    ID3D11Texture2D* depthStencilBuffer_;

    int width_;
    int height_;

    void LoadVertexShader(std::string shaderName);
    void LoadPixelShader(std::string shaderName);
    void LoadModel(std::string modelName);
    void LoadTexture(std::string textureName);

private:
    void LoadMesh(std::string modelName, RawMesh mesh, int meshIndex);
};