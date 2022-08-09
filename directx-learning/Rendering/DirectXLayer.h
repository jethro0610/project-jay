#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <unordered_map>
#include <string>

class DirectXLayer {
public:
    DirectXLayer(HWND windowHandle, int width, int height);
    ~DirectXLayer();

    std::unordered_map<std::string, ID3D11VertexShader*> vertexShaders_;
    std::unordered_map<std::string, ID3DBlob*> vertexShaderBlobs_;
    std::unordered_map<std::string, ID3D11InputLayout*> inputLayouts_;

    std::unordered_map<std::string, ID3D11PixelShader*> pixelShaders_;
    std::unordered_map<std::string, ID3DBlob*> pixelShaderBlobs_;

    std::unordered_map<std::string, ID3D11Buffer*> vertexBuffers_;
    std::unordered_map<std::string, ID3D11Buffer*> indexBuffers_;
    std::unordered_map<std::string, UINT> vertexCounts_;
    std::unordered_map<std::string, UINT> indexCounts_;

    void LoadVertexShader(std::string shaderName);
    void LoadPixelShader(std::string shaderName);
    void LoadMesh(std::string meshName);

    ID3D11Device* device_;
    IDXGISwapChain* swapChain_;
    ID3D11DeviceContext* context_;
    ID3D11RenderTargetView* renderTarget_;

    ID3D11Buffer* perObjectCBuffer_;

    int renderWidth_;
    int renderHeight_;
};