#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <unordered_set>
#include "Resource/ResourceStore.h"
#include "Rendering/Shader.h"
#include "Rendering/Model.h"
#include "Rendering/Skeleton.h"
#include "Rendering/Material.h"
#include "Rendering/Texture.h"
#include "Resource/DependencyList.h"

class ResourceManager {
public:
    static constexpr int MAX_SHADERS = 128;
    static constexpr int MAX_TEXTURES = 32;
    static constexpr int MAX_MODELS = 32;
    static constexpr int MAX_SKELETONS = 32;

    ResourceManager();

    void LoadAllShaders();
    void LoadTexture(const std::string& name);
    void LoadModel(const std::string& name);
    void LoadSkeleton(const std::string& name);

    void UnloadTexture(const std::string& name);
    void UnloadModel(const std::string& name);
    void UnloadSkeleton(const std::string& name);

    Shader* GetShader(const std::string& vertexShaderName, const std::string& fragmentShaderName);
    Model* GetModel(const std::string& name);
    Skeleton* GetSkeleton(const std::string& name);
    Texture* GetTexture(const std::string& name);

    bool HasShader(const std::string& vertexShaderName, const std::string& fragmentShaderName);
    bool HasModel(const std::string& name);
    bool HasSkeleton(const std::string& name);
    bool HasTexture(const std::string& name);

    void LoadDependencies(DependencyList& depdencyList);
    void UnloadUnusedDependencies(DependencyList& depdencyList);

    void UpdateTerrainMapTexture(glm::vec2* terrainMap);

private:
    void LoadGlobals();
    void LoadRenderTextures();
    void CreateTerrainMapTexture();
    void LoadGlobalQuad();
    void LoadGlobalTerrain();

    std::unordered_set<std::string> globals_;

    ResourceStore<Shader, MAX_SHADERS> shaders_;
    ResourceStore<Texture, MAX_TEXTURES> textures_;
    ResourceStore<Model, MAX_MODELS> models_;
    ResourceStore<Skeleton, MAX_SKELETONS> skeletons_;
};
