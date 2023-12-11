#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <unordered_set>
#include "Resource/ResourceStore.h"
#include "Rendering/VertexShader.h"
#include "Rendering/FragmentShader.h"
#include "Rendering/Model.h"
#include "Rendering/Skeleton.h"
#include "Rendering/Material.h"
#include "Rendering/Texture.h"
#include "Entity/EntityDescription.h"
#include "Types/Noise.h"
#include "Particle/ParticleEmitter.h"
#include "Resource/DependencyList.h"

class ResourceManager {
public:
    static constexpr int MAX_VERTEX_SHADERS = 32;
    static constexpr int MAX_FRAGMENT_SHADERS = 32;
    static constexpr int MAX_TEXTURES = 32;
    static constexpr int MAX_MATERIALS = 32;
    static constexpr int MAX_MODELS = 32;
    static constexpr int MAX_SKELETONS = 32;
    static constexpr int MAX_ENTIY_DESCRIPTIONS = 32;
    static constexpr int MAX_EMITTER_PROPERTIES = 32;

    ResourceManager();

    void LoadVertexShader(const std::string& name);
    void LoadFragmentShader(const std::string& name);
    void LoadTexture(const std::string& name);
    void LoadMaterial(const std::string& name);
    void LoadModel(const std::string& name);
    void LoadSkeleton(const std::string& name);
    void LoadEntityDescription(const std::string& name);
    void LoadEmitterProperties(const std::string& name);

    void UnloadVertexShader(const std::string& name);
    void UnloadFragmentShader(const std::string& name);
    void UnloadTexture(const std::string& name);
    void UnloadMaterial(const std::string& name);
    void UnloadModel(const std::string& name);
    void UnloadSkeleton(const std::string& name);
    void UnloadEntityDescription(const std::string& name);
    void UnloadEmitterProperties(const std::string& name);

    VertexShader* GetVertexShader(const std::string& name);
    FragmentShader* GetFragmentShader(const std::string& name);
    Material* GetMaterial(const std::string& name);
    Model* GetModel(const std::string& name);
    Skeleton* GetSkeleton(const std::string& name);
    EntityDescription* GetEntityDescription(const std::string& name);
    EmitterProperties* GetEmitterProperties(const std::string& name);
    Texture* GetTexture(const std::string& name);

    void LoadDependencies(DependencyList& depdencyList);
    void UnloadUnusedDependencies(DependencyList& depdencyList);

    Noise noise_;

private:
    void LoadGlobals();
    void LoadGlobalFile();
    void LoadRenderTextures();
    void LoadGlobalQuad();
    void LoadGlobalTerrain();

    std::unordered_set<std::string> globals_;

    // Kinda uneven that these are handles, maybe wrap them like textures
    ResourceStore<VertexShader, MAX_VERTEX_SHADERS> vertexShaders_;
    ResourceStore<FragmentShader, MAX_FRAGMENT_SHADERS> fragmentShaders_;
    ResourceStore<Texture, MAX_TEXTURES> textures_;
    ResourceStore<Material, MAX_MATERIALS> materials_;
    ResourceStore<Model, MAX_MODELS> models_;
    ResourceStore<Skeleton, MAX_SKELETONS> skeletons_;
    ResourceStore<EntityDescription, MAX_ENTIY_DESCRIPTIONS> entityDescs_;
    ResourceStore<EmitterProperties, MAX_EMITTER_PROPERTIES> emitterProps_;
};
