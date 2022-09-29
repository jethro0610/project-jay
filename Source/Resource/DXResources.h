#pragma once
#include "../Windows/WindowsInc.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <WICTextureLoader.h>
#include <unordered_map>
#include <string>
#include "../Resource/RawModel.h"
#include "../Rendering/RenderTypes.h"
#include "../Game/WorldConstants.h"

// Keep shader and layout together since they're accessed at the same time
// Redundant resource structs with single elements are used for consistency with the maps
struct VSLayout {
    ID3D11VertexShader* shader;
    ID3D11InputLayout* layout;
};

struct DXMesh {
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;
    UINT vertexCount;
    UINT indexCount;
};

enum class VertexShaderType {
    STATIC,
    SKELETAL,
    WORLD
};

class DXResources {
public:
    DXResources(HWND windowHandle, int width, int height);

    int width_;
    int height_;

    ID3D11Device* device_;
    IDXGISwapChain* swapChain_;
    ID3D11DeviceContext* context_;
    ID3D11RenderTargetView* renderTarget_;
    ID3D11SamplerState* textureSampler_;
    ID3D11DepthStencilView* depthStencilBuffer_;

    ID3D11Texture2D* pRenderTexture_;
    ID3D11RenderTargetView* pRenderTarget_;
    ID3D11ShaderResourceView* pRenderTextureResource_;

    std::unordered_map<std::string, VSLayout> vertexShaders_;
    std::unordered_map<std::string, ID3D11PixelShader*> pixelShaders_;
    std::unordered_map<std::string, DXMesh> staticMeshes_;
    std::unordered_map<std::string, DXMesh> skeletalMeshes_;
    std::unordered_map<std::string, ID3D11ShaderResourceView*> textures_;
    DXMesh worldMeshes_[MAX_X_COORDINATES][MAX_Y_COORDINATES][MAX_Z_COORDINATES];

    D3D11_INPUT_ELEMENT_DESC worldVertexDescription_[2];
    D3D11_INPUT_ELEMENT_DESC staticVertexDescription_[5];
    D3D11_INPUT_ELEMENT_DESC skeletalVertexDescription_[7];

    ID3D11Buffer* perFrameCBuffer_;
    ID3D11Buffer* perObjectCBuffer_;
    ID3D11Buffer* perSkeletonCBuffer_;

    ID3D11ComputeShader* computeWVertsShader_;
    ID3D11Buffer* computeWVertsBuffer_;
    ID3D11UnorderedAccessView* computeWVertsView_;
    ID3D11Buffer* computeWVertsOutput_;
    ID3D11Buffer* distanceCacheBuffer_;
    ID3D11ShaderResourceView* distanceCacheView_;

    void LoadVertexShader(std::string shaderName, VertexShaderType shaderType = VertexShaderType::STATIC);
    void LoadPixelShader(std::string shaderName);
    void LoadTexture(std::string textureName);
    void WriteWorldMesh(ivec3 coordinates, const std::vector<WorldVertex>& vertices, const std::vector<uint16_t>& indices);

    // Should only be calling this from ResourceManager
    void LoadRawModel(RawModel& rawModel, std::string modelName, bool skeletal = false);

private:
    void CreateConstantBuffer(int size, ID3D11Buffer** outBuffer);
    void CreateInputStructuredBufferAndView(int elementSize, int numberOfElements, ID3D11Buffer** outBuffer, ID3D11ShaderResourceView** outView);
    void CreateOutputStructuredBufferAndView(int elementSize, int numberOfElements, ID3D11Buffer** outBuffer, ID3D11UnorderedAccessView** outView, ID3D11Buffer** outStagingBuffer);

    void LoadMesh(std::string modelName, RawMesh mesh, int meshIndex, bool skeletal);
    void InitWorldMeshes();
    WorldVertex coordinateFillVertices_[MAX_COORDINATE_VERTICES];
    uint16_t coordinateFillIndices_[MAX_COORDINATE_INDICES];
};
