#include "ResourceManager.h"

ResourceManager::ResourceManager(DXResources& dxResources):
    dxResources_(dxResources)
{ }

void ResourceManager::GPULoadStaticModel_P(RawModel& rawModel, std::string modelName) {
    assert(staticModels_.count(modelName) == 0);
    dxResources_.LoadRawModel(rawModel, modelName, false);
}
