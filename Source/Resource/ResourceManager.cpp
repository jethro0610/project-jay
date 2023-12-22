#include "ResourceManager.h"

void ResourceManager::LoadDependencies(DependencyList& dependencyList) {
    for(const std::string& name : dependencyList.vertexShaders) {
        if (!vertexShaders_.GetMap().contains(name))
            LoadVertexShader(name);
    }

    for(const std::string& name : dependencyList.fragmentShaders) {
        if (!fragmentShaders_.GetMap().contains(name))
            LoadFragmentShader(name);
    }

    for(const std::string& name : dependencyList.textures) {
        if (!textures_.GetMap().contains(name))
            LoadTexture(name);
    }

    for(const std::string& name : dependencyList.materials) {
        if (!materials_.GetMap().contains(name))
            LoadMaterial(name);
    }

    for(const std::string& name : dependencyList.models) {
        if (!models_.GetMap().contains(name))
            LoadModel(name);
    }

    for(const std::string& name : dependencyList.entityDescriptions) {
        if (!entityDescs_.GetMap().contains(name))
            LoadEntityDescription(name);
    }

    for(const std::string& name : dependencyList.emitterProperties) {
        if (!emitterProps_.GetMap().contains(name))
            LoadEmitterProperties(name);
    }
}

void ResourceManager::UnloadUnusedDependencies(DependencyList& dependencyList) {
    std::vector<std::string> unloads;

    for(const auto& element : vertexShaders_.GetMap()) {
        if (!dependencyList.vertexShaders.contains(element.first) && !globals_.contains(element.first))
            unloads.push_back(element.first);
    }
    for (const std::string& unload : unloads)
        UnloadVertexShader(unload);
    unloads.clear();

    for(const auto& element : fragmentShaders_.GetMap()) {
        if (!dependencyList.fragmentShaders.contains(element.first) && !globals_.contains(element.first))
            unloads.push_back(element.first);
    }
    for (const std::string& unload : unloads)
        UnloadFragmentShader(unload);
    unloads.clear();

    for(const auto& element : textures_.GetMap()) {
        if (!dependencyList.textures.contains(element.first) && !globals_.contains(element.first))
            unloads.push_back(element.first);
    }
    for (const std::string& unload : unloads)
        UnloadTexture(unload);
    unloads.clear();

    for(const auto& element : materials_.GetMap()) {
        if (!dependencyList.materials.contains(element.first) && !globals_.contains(element.first))
            unloads.push_back(element.first);
    }
    for (const std::string& unload : unloads)
        UnloadMaterial(unload);
    unloads.clear();

    for(const auto& element : models_.GetMap()) {
        if (!dependencyList.models.contains(element.first) && !globals_.contains(element.first))
            unloads.push_back(element.first);
    }
    for (const std::string& unload : unloads)
        UnloadModel(unload);
    unloads.clear();

    for(const auto& element : entityDescs_.GetMap()) {
        if (!dependencyList.entityDescriptions.contains(element.first) && !globals_.contains(element.first))
            unloads.push_back(element.first);
    }
    for (const std::string& unload : unloads)
        UnloadEntityDescription(unload);
    unloads.clear();

    for(const auto& element : emitterProps_.GetMap()) {
        if (!dependencyList.emitterProperties.contains(element.first) && !globals_.contains(element.first))
            unloads.push_back(element.first);
    }
    for (const std::string& unload : unloads)
        UnloadEmitterProperties(unload);
    unloads.clear();
}

VertexShader* ResourceManager::GetVertexShader(const std::string& name) {
    return &vertexShaders_.Get(name);
}

FragmentShader* ResourceManager::GetFragmentShader(const std::string& name) {
    return &fragmentShaders_.Get(name);
}

Texture* ResourceManager::GetTexture(const std::string& name) {
    return &textures_.Get(name);
}

Material* ResourceManager::GetMaterial(const std::string& name) {
    return &materials_.Get(name);
}

Model* ResourceManager::GetModel(const std::string& name) {
    return &models_.Get(name);
}

Skeleton* ResourceManager::GetSkeleton(const std::string& name) {
    return &skeletons_.Get(name);
}

EntityDescription* ResourceManager::GetEntityDescription(const std::string& name) {
    return &entityDescs_.Get(name);
}

EmitterProperties* ResourceManager::GetEmitterProperties(const std::string& name) {
    return &emitterProps_.Get(name);
}

bool ResourceManager::HasVertexShader(const std::string& name) {
    return vertexShaders_.Has(name);
}

bool ResourceManager::HasFragmentShader(const std::string& name) {
    return fragmentShaders_.Has(name);
}

bool ResourceManager::HasTexture(const std::string& name) {
    return textures_.Has(name);
}

bool ResourceManager::HasMaterial(const std::string& name) {
    return materials_.Has(name);
}

bool ResourceManager::HasModel(const std::string& name) {
    return models_.Has(name);
}

bool ResourceManager::HasSkeleton(const std::string& name) {
    return skeletons_.Has(name);
}

bool ResourceManager::HasEntityDescription(const std::string& name) {
    return entityDescs_.Has(name);
}

bool ResourceManager::HasEmitterProperties(const std::string& name) {
    return emitterProps_.Has(name);
}
