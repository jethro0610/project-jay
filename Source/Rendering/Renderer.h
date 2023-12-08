#pragma once
#ifdef _PC
#include <GLFW/glfw3.h>
#endif

#include <array>
#include <glm/mat4x4.hpp>
#include <string>
#include <unordered_map>
#include "RenderDefs.h"
#include "Animation.h"
#include "Skeleton.h"
#include "Model.h"
#include "Material.h"

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
    Renderer(ResourceManager& resourceManager, Noise& noise, GLFWwindow* window);
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

    const Texture& noiseTexture_;

    const Model& spreadModel_;
    const Model& quadModel_;
    const Model& terrainModel_;

    const Material& postProcessMaterial_;
    const Material& terrainMaterial_;
    const Material& blitMaterial_;

    // std::array<Material, 2> spreadMaterials_;
    // Material seedMaterial_;
    // Material textMaterial_;
    // Material barMaterial_;
    // Material blitMaterial_;
    //

    void InitShadowBuffer(const Texture& shadowBufferTexture);
    void InitRenderBuffer(const Texture& renderColorTexture, const Texture& renderDepthTexture);
    void InitPostProcessBuffer(const Texture& postProcessTexture);
    void InitUIBuffer_P();

    FrameBuffer backBuffer_;
    FrameBuffer shadowBuffer_;
    FrameBuffer renderBuffer_;
    FrameBuffer postProcessBuffer_;
    // Texture shadowBufferTexture_;
    // std::array<Texture, 2> renderBufferTextures_;
    // Texture postProcessTexture_;

    std::array<TextureSampler, MAX_TEXTURES_PER_MATERIAL> samplers_;
    TextureSampler shadowSampler_;
    TextureSampler worldNoiseSampler_;

    Uniform u_shadowUp_;
    Uniform u_shadowRight_;
    Uniform u_shadowMatrix_;
    Uniform u_shadowResolution_;
    Uniform u_pose_;
    Uniform u_materialProps_;
    Uniform u_particleProps_;
    Uniform u_normalMult_;
    Uniform u_lightDirection_;
    Uniform u_timeResolution_;
    Uniform u_cameraPosition_;
    Uniform u_cameraUp_;
    Uniform u_cameraRight_;
    Uniform u_randomVec_;
    Uniform u_meter_;
    Uniform u_worldProps_;
    Uniform u_worldMeshOffset_;
    Uniform u_noiseProps_;

    // Texture MakeNoiseTexture_P(Noise& noise);

    const ShaderResources& vertexShaders_;
    const ShaderResources& fragmentShaders_;
    const ModelResources& models_;
    const SkeletonResources& skeletons_;
    const TextureResources& textures_;
    const MaterialResources& materials_;

    glm::mat4 GetModelViewProjection(const glm::mat4& modelMatrix);
    void SetTexturesFromMaterial_P(Material& material, bool shadowMap = true);
    void SetLightDirection_P(glm::vec3 direction);

    void StartFrame_P();
    void Clear_P();
    void RenderMesh_P(
        Mesh& mesh, 
        Material& material, 
        InstanceBuffer* instanceBuffer = nullptr, 
        glm::mat4* modelMatrix = nullptr,
        GPUPose* pose = nullptr 
    );
    void RenderWorld_P(World& world);
    void RenderEntities_P(
        EntityList& entities, 
        ComponentList& components
    );
    void RenderSpread_P(SpreadManager& spreadManager);
    void RenderSeed_P(SeedManager& seedManager);
    void RenderParticles_P(ParticleManager& particleManager);
    void RenderPostProcess_P();
    void RenderBlit_P();
    void RenderUI_P(ComponentList& components);
    #ifdef _DEBUG
    void RenderScreenText_P();
    #endif
    void PresentFrame_P();
};

