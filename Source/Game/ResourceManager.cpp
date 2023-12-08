#include "ResourceManager.h"
#include "../Helpers/MapCheck.h"

ShaderHandle ResourceManager::GetVertexShader(const std::string& name) {
    return GetFromMap<ShaderHandle>(vertexShaders_, name, "Tried using unloaded vertex shader " + name);
}

ShaderHandle ResourceManager::GetFragmentShader(const std::string& name) {
    return GetFromMap<ShaderHandle>(fragmentShaders_, name, "Tried using unloaded fragment shader " + name);
}

TextureHandle ResourceManager::GetTexture(const std::string& name) {
    return GetFromMap<TextureHandle>(textures_, name, "Tried using unloaded texture " + name);
}

Model* ResourceManager::GetModel(const std::string& name) {
    return &GetFromMap<Model>(models_, name, "Tried using unloaded model " + name);
}

Skeleton* ResourceManager::GetSkeleton(const std::string& name) {
    return &GetFromMap<Skeleton>(skeletons_, name, "Tried using unloaded skeleton " + name);
}

Material* ResourceManager::GetMaterial(const std::string& name) {
    return &GetFromMap<Material>(materials_, name, "Tried using unloaded material " + name);
}

nlohmann::json* ResourceManager::GetEntityDescription(const std::string& name) {
    return &GetFromMap<nlohmann::json>(entityDescs_, name, "Tried using unloaded entity description " + name);
}

EmitterProperties* ResourceManager::GetEmitterProperties(const std::string& name) {
    return &GetFromMap<EmitterProperties>(emitterProps_, name, "Tried using unloaded emitter properties " + name);
}
