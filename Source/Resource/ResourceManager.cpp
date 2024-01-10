#include "ResourceManager.h"

void ResourceManager::LoadDependencies(DependencyList& dependencyList) {
    for(const std::string& name : dependencyList.textures) {
        if (!textures_.GetMap().contains(name))
            LoadTexture(name);
    }

    for(const std::string& name : dependencyList.models) {
        if (!models_.GetMap().contains(name))
            LoadModel(name);
    }
}

void ResourceManager::UnloadUnusedDependencies(DependencyList& dependencyList) {
    std::vector<std::string> unloads;

    for(const auto& element : textures_.GetMap()) {
        if (!dependencyList.textures.contains(element.first) && !globals_.contains(element.first))
            unloads.push_back(element.first);
    }
    for (const std::string& unload : unloads)
        UnloadTexture(unload);
    unloads.clear();

    for(const auto& element : models_.GetMap()) {
        if (!dependencyList.models.contains(element.first) && !globals_.contains(element.first))
            unloads.push_back(element.first);
    }
    for (const std::string& unload : unloads)
        UnloadModel(unload);
    unloads.clear();
}

Shader* ResourceManager::GetShader(const std::string& vertexShaderName, const std::string& fragmentShaderName) {
    return &shaders_.Get(vertexShaderName + '.' + fragmentShaderName);
}

Texture* ResourceManager::GetTexture(const std::string& name) {
    return &textures_.Get(name);
}

Model* ResourceManager::GetModel(const std::string& name) {
    return &models_.Get(name);
}

Skeleton* ResourceManager::GetSkeleton(const std::string& name) {
    return &skeletons_.Get(name);
}

bool ResourceManager::HasShader(const std::string& vertexShaderName, const std::string& fragmentShaderName) {
    return shaders_.Has(vertexShaderName + fragmentShaderName);
}

bool ResourceManager::HasTexture(const std::string& name) {
    return textures_.Has(name);
}

bool ResourceManager::HasModel(const std::string& name) {
    return models_.Has(name);
}

bool ResourceManager::HasSkeleton(const std::string& name) {
    return skeletons_.Has(name);
}
