#include "ResourceManager.h"
#include "../Helpers/MapCheck.h"

const Shader& ResourceManager::GetVertexShader(const std::string& name) {
    return GetFromMap<Shader>(vertexShaders_, name, "Tried using unloaded vertex shader " + name);
}

const Shader& ResourceManager::GetFragmentShader(const std::string& name) {
    return GetFromMap<Shader>(fragmentShaders_, name, "Tried using unloaded fragment shader " + name);
}

const Model& ResourceManager::GetModel(const std::string& name) {
    return GetFromMap<Model>(models_, name, "Tried using unloaded model " + name);
}

const Skeleton& ResourceManager::GetSkeleton(const std::string& name) {
    return GetFromMap<Skeleton>(skeletons_, name, "Tried using unloaded skeleton " + name);
}

const Texture& ResourceManager::GetTexture(const std::string& name) {
    return GetFromMap<Texture>(textures_, name, "Tried using unloaded texture " + name);
}

const Material& ResourceManager::GetMaterial(const std::string& name) {
    return GetFromMap<Material>(materials_, name, "Tried using unloaded material " + name);
}

const nlohmann::json& ResourceManager::GetEntityDescription(const std::string& name) {
    return GetFromMap<nlohmann::json>(entityDescs_, name, "Tried using unloaded entity description " + name);
}

const EmitterProperties& ResourceManager::GetEmitterProperties(const std::string& name) {
    return GetFromMap<EmitterProperties>(emitterProps_, name, "Tried using unloaded emitter properties " + name);
}
