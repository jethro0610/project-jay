#pragma once
#include <json.hpp>
#include <unordered_map>
#include "RawModel.h"

#ifdef _WINDOWS
#include "DXResources.h"
#endif

struct StaticModelDesc {
    int meshCount;
};

const uint8_t MAX_MATERIAL_TEXTURES = 8;

struct MaterialDesc {
    std::string vertexShader;
    std::string pixelShader;
    std::string textures[MAX_MATERIAL_TEXTURES];
    int numOfTextures;
};

class ResourceManager {
public:
#ifdef _WINDOWS
    ResourceManager(DXResources& dxResources);
    DXResources& dxResources_;
#endif
    std::unordered_map<std::string, StaticModelDesc> staticModels_;
    std::unordered_map<std::string, MaterialDesc> materials_;
    std::unordered_map<std::string, nlohmann::json> entities_;

    void LoadStaticModel(std::string modelName);
    void LoadEntity(std::string entityName);
private:
    void GPULoadStaticModel_P(RawModel& rawModel, std::string modelName);
};

