#include "ResourceManager.h"

void ResourceManager::LoadStaticModel(std::string modelName) {
    assert(staticModels_.count(modelName) == 0);
    StaticModelDesc description;
    RawModel rawModel((modelName + ".jmd").c_str(), false);
    description.meshCount = rawModel.meshes_.size();
    GPULoadStaticModel_P(rawModel, modelName);
    staticModels_[modelName] = description;
}