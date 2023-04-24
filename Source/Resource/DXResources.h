#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <WICTextureLoader.h>
#include <unordered_map>
#include <string>
#include <assert.h>
#include "../Resource/RawModel.h"
#include "../Rendering/RenderTypes.h"
#include "../Constants/WorldConstants.h"
#include "../Constants/SpreadConstants.h"
#include "../Logging/LogConstants.h"
#include "../Windows/WindowsInc.h"

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

struct WorldMesh {
    ID3D11Buffer* vertexBuffer;
    UINT vertexCount;
};

enum class VertexShaderType {
    STATIC,
    SKELETAL,
    WORLD,
    INSTANCE
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

    ID3D11BlendState* noBlendState_;
    ID3D11BlendState* alphaBlendState_;

    ID3D11Buffer* textBuffer_;
    ID3D11VertexShader* textVS_;
    ID3D11InputLayout* textVSLayout_;
    ID3D11PixelShader* textPS_; 
    ID3D11ShaderResourceView* textTexture_;

    D3D11_INPUT_ELEMENT_DESC worldVertexDescription_[2];
    D3D11_INPUT_ELEMENT_DESC staticVertexDescription_[5];
    D3D11_INPUT_ELEMENT_DESC skeletalVertexDescription_[7];
    D3D11_INPUT_ELEMENT_DESC instanceVertexDescription_[6];

    ID3D11Buffer* perFrameCBuffer_;
    ID3D11Buffer* perObjectCBuffer_;
    ID3D11Buffer* perSkeletonCBuffer_;
    ID3D11Buffer* perChunkCBuffer_;
    ID3D11Buffer* perUICBuffer_;

    ID3D11Texture2D* noiseTexture_;
    ID3D11ShaderResourceView* noiseTextureSRV_;

    WorldMesh worldMeshes_[MAX_X_CHUNKS][MAX_Y_CHUNKS][MAX_Z_CHUNKS];
    ID3D11Buffer* spreadBuffers_[MAX_X_CHUNKS][MAX_Z_CHUNKS];
    ID3D11ComputeShader* csWorldVertex_;
    ID3D11Buffer* csWorldVertexBuffer_;
    ID3D11UnorderedAccessView* csWorldVertexView_;
    ID3D11Buffer* csWorldCountBuffer_;
    ID3D11UnorderedAccessView* csWorldCountView_;
    ID3D11Buffer* csWorldCountOutput_;

    ID3D11Buffer* terrainModBuffer_;
    ID3D11ShaderResourceView* terrainModSRV_;

    void LoadVertexShader(std::string shaderName, VertexShaderType shaderType = VertexShaderType::STATIC);
    void LoadPixelShader(std::string shaderName);
    void LoadTexture(std::string textureName);

    // Should only be calling this from ResourceManager
    void LoadRawModel(RawModel& rawModel, std::string modelName, bool skeletal = false);

    void UpdateBuffer(ID3D11Buffer* buffer, void* data, int size);

private:
    void CreateConstantBuffer(int size, ID3D11Buffer** outBuffer);
    void CreateRWStructuredBufferAndUAV(
        int elementSize, 
        int numberOfElements, 
        ID3D11Buffer** outBuffer, 
        ID3D11UnorderedAccessView** outView, 
        bool append,
        ID3D11Buffer** outStagingBuffer
    );
    void CreateStructuredBufferAndSRV(
        int elementSize, 
        int numberOfElements, 
        ID3D11Buffer** outBuffer, 
        ID3D11ShaderResourceView** outView
    );

    void LoadMesh(std::string modelName, RawMesh mesh, int meshIndex, bool skeletal);
    void InitWorldMeshes();
    void InitSpreadBuffers();
    void InitText();
    void InitNoiseTexture();

    WorldTri chunkFillTris_[MAX_CHUNK_TRIS];
};
