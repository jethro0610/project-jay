#pragma once
#include <array>
#include <glm/mat4x4.hpp>
#include <string>
#include <unordered_map>
#include "RenderDefs.h"
#include "Animation.h"
#include "Skeleton.h"
#include "Model.h"
#include "Material.h"
#include "Texture.h"

#include "../Game/Entity/EntityList.h"
#include "../Game/Components/ComponentList.h"
#include "../Game/ResourceManager.h"

#ifdef _DEBUG
#include "../Logging/ScreenText.h"
#endif

class Camera;
class Noise;
class SeedManager;
class SpreadManager;
class World;
class ParticleManager;

class Renderer {
public:
    #ifdef _PC
    Renderer(ResourceManager& resourceManager);
    #endif
    Camera* camera_;

    void Render(
        EntityList& entities,
        ComponentList& components,
        ParticleManager& particleManager,
        SeedManager& seedManager,
        SpreadManager& spreadManager,
        World& world
    );

private:
    int width_;
    int height_;
    int renderWidth_;
    int renderHeight_;

    glm::vec3 lightDirection_;
    glm::mat4 viewMatrix_;
    glm::mat4 projectionMatrix_;
    glm::mat4 shadowViewMatrix_;
    glm::mat4 shadowProjectionMatrix_;
    glm::mat4 shadowMatrix_;

    Texture* noiseTexture_;

    Model* spread_;
    Mesh* quad_;
    Mesh* terrain_;

    std::array<Material*, Model::MAX_MESHES_PER_MODEL> spreadMaterials_;
    Material* terrainMaterial_;
    Material* seedMaterial_;
    Material* barMaterial_;
    Material* blitMaterial_;
    Material* postProcessMaterial_;
    Material* textMaterial_;

    void InitShadowBuffer(TextureHandle shadowBufferTexture);
    void InitRenderBuffer(TextureHandle renderColorTexture, TextureHandle renderDepthTexture);
    void InitPostProcessBuffer(TextureHandle postProcessTexture);
    void InitUIBuffer();

    FrameBufferHandle backBuffer_;
    FrameBufferHandle shadowBuffer_;
    FrameBufferHandle renderBuffer_;
    FrameBufferHandle postProcessBuffer_;

    std::array<TextureHandle, 2> renderBufferTextures_;
    TextureHandle shadowBufferTexture_;
    TextureHandle postProcessTexture_;

    std::array<TextureSamplerHandle, MAX_TEXTURES_PER_MATERIAL> samplers_;
    TextureSamplerHandle shadowSampler_;
    TextureSamplerHandle worldNoiseSampler_;

    UniformHandle u_shadowUp_;
    UniformHandle u_shadowRight_;
    UniformHandle u_shadowMatrix_;
    UniformHandle u_shadowResolution_;
    UniformHandle u_pose_;
    UniformHandle u_materialProps_;
    UniformHandle u_particleProps_;
    UniformHandle u_normalMult_;
    UniformHandle u_lightDirection_;
    UniformHandle u_timeResolution_;
    UniformHandle u_cameraPosition_;
    UniformHandle u_cameraUp_;
    UniformHandle u_cameraRight_;
    UniformHandle u_randomVec_;
    UniformHandle u_meter_;
    UniformHandle u_worldProps_;
    UniformHandle u_worldMeshOffset_;
    UniformHandle u_noiseProps_;

    glm::mat4 GetModelViewProjection(const glm::mat4& modelMatrix);
    void SetTexturesFromMaterial(Material* material, bool shadowMap = true);
    void SetLightDirection(const glm::vec3& direction);

    void StartFrame();
    void Clear();
    void RenderMesh(
        Mesh* mesh, 
        Material* material, 
        InstanceBufferHandle* instanceBuffer = nullptr, 
        glm::mat4* modelMatrix = nullptr,
        GPUPose* pose = nullptr 
    );
    void RenderWorld(World& world);
    void RenderEntities(
        EntityList& entities, 
        ComponentList& components
    );
    void RenderSpread(SpreadManager& spreadManager);
    void RenderSeed(SeedManager& seedManager);
    void RenderParticles(ParticleManager& particleManager);
    void RenderPostProcess();
    void RenderBlit();
    void RenderUI(ComponentList& components);
    #ifdef _DEBUG
    void RenderScreenText();
    #endif
    void PresentFrame();
};

