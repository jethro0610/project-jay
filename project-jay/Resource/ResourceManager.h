#pragma once
#include <unordered_map>

#ifdef WINDOWS
#include "DXResources.h"
#endif

struct StaticModelDescription {
    int meshCount;
};

class ResourceManager {
public:
#ifdef WINDOWS
    ResourceManager(DXResources* dxResources);
    DXResources* dxResources_;
#endif
    std::unordered_map<std::string, StaticModelDescription> loadedStaticModels_;
    void LoadStaticModel(std::string modelName);

private:
    void GPULoadStaticModel_P(RawModel& rawModel, std::string modelName);
};

