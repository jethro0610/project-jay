#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "GraphicsTypes.h"

class Rendering {
public:
    Rendering(HWND windowHandle, int width, int height);
    void Draw();

private:
    ID3D11Device* device_;
    IDXGISwapChain* swapChain_;
    ID3D11DeviceContext* context_;
    ID3D11RenderTargetView* backBufferTarget_;
    ID3D11Texture2D* depthStencilBuffer_;
    ID3D11DepthStencilView* depthStencilView_;

    ID3D11InputLayout* inputLayout_;

    ID3DBlob* vertexShaderBlob_ = nullptr;
    ID3D11VertexShader* vertexShader_;
    ID3DBlob* pixelShaderBlob_ = nullptr;
    ID3D11PixelShader* pixelShader_;

    ID3D11Buffer* vertexBuffer_;

    UINT vertexStride_;
    UINT vertexOffset_;
    UINT vertexCount_;

    int renderWidth_;
    int renderHeight_;
};