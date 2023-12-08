#include <string>
#include <nlohmann/json.hpp>
#include <unordered_set>
#include "../Rendering/RenderDefs.h"
#include "../Rendering/Model.h"
#include "../Rendering/Skeleton.h"
#include "../Rendering/Material.h"
#include "../Types/Noise.h"
#include "ParticleEmitter.h"

typedef std::unordered_map<std::string, Shader> ShaderResources;
typedef std::unordered_map<std::string, Texture> TextureResources;
typedef std::unordered_map<std::string, Material> MaterialResources;
typedef std::unordered_map<std::string, Model> ModelResources;
typedef std::unordered_map<std::string, Skeleton> SkeletonResources;
typedef std::unordered_map<std::string, nlohmann::json> EntityDescResources;
typedef std::unordered_map<std::string, EmitterProperties> EmitterPropResources;

class ResourceManager {
public:
    ResourceManager();

    void LoadGlobals();
    void LoadRenderTextures();
    void LoadGlobalQuad();
    void LoadGlobalTerrain();

    void LoadVertexShader(const std::string& name);
    void LoadFragmentShader(const std::string& name);
    void LoadTexture(const std::string& name);
    void LoadMaterial(const std::string& name);
    void LoadModel(const std::string& name);
    void LoadSkeleton(const std::string& name);
    void LoadEntityDescription(const std::string& name);
    void LoadEmitterProperties(const std::string& name);

    const Material& GetMaterial(const std::string& name);
    const Model& GetModel(const std::string& name);
    const Skeleton& GetSkeleton(const std::string& name);
    const nlohmann::json& GetEntityDescription(const std::string& name);
    const EmitterProperties& GetEmitterProperties(const std::string& name);

private:
    // Since these are accessed via higher level structs,
    // they are kept private
    const Shader& GetVertexShader(const std::string& name);
    const Shader& GetFragmentShader(const std::string& name);
    const Texture& GetTexture(const std::string& name);

    std::unordered_set<std::string> globals_;
    Noise noise_;
    ShaderResources vertexShaders_;
    ShaderResources fragmentShaders_;
    TextureResources textures_;
    MaterialResources materials_;
    ModelResources models_;
    SkeletonResources skeletons_;
    EntityDescResources entityDescs_;
    EmitterPropResources emitterProps_;
};
