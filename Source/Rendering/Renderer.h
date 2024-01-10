#pragma once
#include "RenderDefs.h"
#include "Animation.h"
#include "Skeleton.h"
#include "Model.h"
#include "Material.h"
#include "Entity/EntityList.h"
#include "Text/Text.h"
#include <array>
#include <glm/mat4x4.hpp>

#ifdef _DEBUG
#include "Logging/ScreenText.h"
#endif

class Camera;
class Editor;
class LevelProperties;
class SeedManager;
class SpreadManager;
class Terrain;
class ParticleManager;
class Player;
class ResourceManager;

class Renderer {
public:
    #ifdef _PC
    Renderer(ResourceManager& resourceManager);
    #endif
    Camera* camera_;

    void Render(
        EntityList& entities,
        LevelProperties& levelProperties,
        ParticleManager& particleManager,
        SeedManager& seedManager,
        SpreadManager& spreadManager,
        Terrain& terrain
    );

    void RenderEdit(
        EntityList& entities,
        Editor& editor,
        LevelProperties& levelProperties,
        Terrain& terrain
    );

    void RenderEditorOnly(
        Editor& editor
    );

    glm::mat4 GetProjectionMatrix() const { return projectionMatrix_; };

private:
    int width_;
    int height_;
    int renderWidth_;
    int renderHeight_;

    Texture* renderBufferTextures_[2];
    Texture* terrainMapTexture_;
    Texture* shadowBufferTexture_;
    Texture* postProcessTexture_;

    Mesh* quad_;
    Mesh* terrain_;

    Material barMaterial_;
    Material blitMaterial_;
    Material postProcessMaterial_;
    Material textMaterial_;

    #ifdef _DEBUG
    Mesh* defaultMesh_;
    Material defaultMaterial_;
    Material defaultSelectedMaterial_;
    #endif

    glm::vec3 lightDirection_;
    glm::mat4 viewMatrix_;
    glm::mat4 projectionMatrix_;
    glm::mat4 shadowViewMatrix_;
    glm::mat4 shadowProjectionMatrix_;
    glm::mat4 shadowMatrix_;

    void InitShadowBuffer(Texture* shadowBufferTexture);
    void InitRenderBuffer(Texture* renderColorTexture, Texture* renderDepthTexture);
    void InitPostProcessBuffer(Texture* postProcessTexture);
    void InitUIBuffer();

    FrameBufferHandle backBuffer_;
    FrameBufferHandle shadowBuffer_;
    FrameBufferHandle renderBuffer_;
    FrameBufferHandle postProcessBuffer_;

    TextureSamplerHandle samplers_[Material::MAX_TEXTURES];
    TextureSamplerHandle shadowSampler_;
    TextureSamplerHandle terrainMapSampler_;

    UniformHandle u_shadowUp_;
    UniformHandle u_shadowRight_;
    UniformHandle u_shadowMatrix_;
    UniformHandle u_shadowResolution_;
    UniformHandle u_pose_;
    UniformHandle u_mProps_;
    UniformHandle u_particleProps_;
    UniformHandle u_normalMult_;
    UniformHandle u_lightDirection_;
    UniformHandle u_timeResolution_;
    UniformHandle u_cameraPosition_;
    UniformHandle u_cameraUp_;
    UniformHandle u_cameraRight_;
    UniformHandle u_randomVec_;
    UniformHandle u_meter_;
    UniformHandle u_terrainMeshOffset_;
    UniformHandle u_textProps_;

    glm::mat4 GetModelViewProjection(const glm::mat4& modelMatrix);
    void SetTexturesFromMaterial(Material* material, bool shadowMap = true);
    void SetLightDirection(const glm::vec3& direction);

    void StartFrame();
    void Clear();
    #ifndef _DEBUG
    void RenderMesh(
        Mesh* mesh, 
        Material* material, 
        InstanceBufferHandle* instanceBuffer = nullptr, 
        glm::mat4* modelMatrix = nullptr,
        GPUPose* pose = nullptr 
    );
    #else
    void RenderMesh(
        Mesh* mesh, 
        Material* material, 
        InstanceBufferHandle* instanceBuffer = nullptr, 
        glm::mat4* modelMatrix = nullptr,
        GPUPose* pose = nullptr,
        bool selected = false
    );
    #endif
    void RenderTerrain(
        Terrain& terrain, 
        Material* material,
        float maxRadius
    );
    void RenderEntitiesS(
        EntityList& entities
    );
    void RenderSpread(
        SpreadManager& spreadManager, 
        Model* model, 
        std::array<Material, Model::MAX_MESHES_PER_MODEL>& materials
    );
    void RenderSeed(SeedManager& seedManagerm, Material* material);
    void RenderParticles(ParticleManager& particleManager);
    void RenderPostProcess();
    void RenderBlit();
    void RenderUI(EntityList& entities);
    void RenderText(Text& text);
    void RenderEditor(Editor& editor);
    #ifdef _DEBUG
    void RenderScreenText();
    #endif
    void PresentFrame();
};

