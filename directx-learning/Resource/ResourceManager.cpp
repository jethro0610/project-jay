#include "ResourceManager.h"

void ResourceManager::LoadStaticModel(std::string modelName) {
    assert(loadedStaticModels_.count(modelName) == 0);
    StaticModelDescription description;
    RawModel rawModel((modelName + ".jmd").c_str(), false);
    description.meshCount = rawModel.meshes_.size();
    GPULoadStaticModel_P(rawModel, modelName);
    loadedStaticModels_[modelName] = description;
}