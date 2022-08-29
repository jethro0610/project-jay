#pragma once
#include <unordered_map>

#ifdef _WINDOWS
#include "DXResources.h"
#endif

struct StaticModelDesc {
    int meshCount;
};

#define MAX_MATERIAL_TEXTURES 8
struct MaterialDesc {
    std::string vertexShader;
    std::string pixelShader;
    std::string textures[MAX_MATERIAL_TEXTURES];
    int numOfTextures;
};

class ResourceManager {
public:
#ifdef _WINDOWS
    ResourceManager(DXResources* dxResources);
    DXResources* dxResources_;
#endif
    std::unordered_map<std::string, StaticModelDesc> staticModels_;
    std::unordered_map<std::string, MaterialDesc> materials_;

    void LoadStaticModel(std::string modelName);
private:
    void GPULoadStaticModel_P(RawModel& rawModel, std::string modelName);
};

