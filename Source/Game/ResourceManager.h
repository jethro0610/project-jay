#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <unordered_set>
#include "../Rendering/RenderDefs.h"
#include "../Rendering/Model.h"
#include "../Rendering/Skeleton.h"
#include "../Rendering/Material.h"
#include "../Rendering/Texture.h"
#include "Entity/EntityDescription.h"
#include "../Types/Noise.h"
#include "ParticleEmitter.h"

typedef std::unordered_map<std::string, ShaderHandle> ShaderResources;

typedef std::unordered_map<std::string, Texture> TextureResources;
typedef std::unordered_map<std::string, Material> MaterialResources;
typedef std::unordered_map<std::string, Model> ModelResources;
typedef std::unordered_map<std::string, Skeleton> SkeletonResources;
typedef std::unordered_map<std::string, EntityDescription> EntityDescResources;
typedef std::unordered_map<std::string, EmitterProperties> EmitterPropResources;

class ResourceManager {
public:
    ResourceManager();

    void LoadVertexShader(const std::string& name);
    void LoadFragmentShader(const std::string& name);
    void LoadTexture(const std::string& name);
    void LoadMaterial(const std::string& name);
    void LoadModel(const std::string& name);
    void LoadSkeleton(const std::string& name);
    void LoadEntityDescription(const std::string& name);
    void LoadEmitterProperties(const std::string& name);

    Material* GetMaterial(const std::string& name);
    Model* GetModel(const std::string& name);
    Skeleton* GetSkeleton(const std::string& name);
    EntityDescription* GetEntityDescription(const std::string& name);
    EmitterProperties* GetEmitterProperties(const std::string& name);
    Texture* GetTexture(const std::string& name);

    Noise noise_;

private:
    void LoadGlobals();
    void LoadRenderTextures();
    void LoadGlobalQuad();
    void LoadGlobalTerrain();

    // Since these are accessed via higher level structs,
    // they are kept private. Note that these are handles,
    // so no reference is used.
    ShaderHandle GetVertexShader(const std::string& name);
    ShaderHandle GetFragmentShader(const std::string& name);

    std::unordered_set<std::string> globals_;

    ShaderResources vertexShaders_;
    ShaderResources fragmentShaders_;
    TextureResources textures_;
    MaterialResources materials_;
    ModelResources models_;
    SkeletonResources skeletons_;
    EntityDescResources entityDescs_;
    EmitterPropResources emitterProps_;
};
