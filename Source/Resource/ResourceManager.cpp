#include "ResourceManager.h"
#include <fstream>
using json = nlohmann::json;

void ResourceManager::LoadStaticModel(std::string modelName) {
    assert(staticModels_.count(modelName) == 0);
    StaticModelDesc description;
    RawModel rawModel((modelName + ".jmd").c_str(), false);
    description.meshCount = int(rawModel.meshes_.size());
    GPULoadStaticModel_P(rawModel, modelName);
    staticModels_[modelName] = description;
}

void ResourceManager::LoadEntity(std::string entityName) {
    assert(entities_.count(entityName) == 0);
    std::ifstream inFile(entityName + ".json");
    json data = json::parse(inFile);
    entities_[entityName] = data;
}
