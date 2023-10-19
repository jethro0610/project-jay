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
    Renderer(Noise& noise, GLFWwindow* window);
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

    Shader& LoadVertexShader_P(std::string name);
    Shader& LoadFragmentShader_P(std::string name);
    Model& LoadModel_P(std::string name);
    Texture& LoadTexture_P(std::string name);
    Material& LoadMaterial_P(std::string name);

    void TEMP_LoadTestData();

    Shader& GetVertexShader(std::string name);
    Shader& GetFragmentShader(std::string name);
    Model& GetModel(std::string name);
    Skeleton& GetSkeleton(std::string name);
    Texture& GetTexture(std::string name);
    Material& GetMaterial(std::string name);

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

    Model spreadModel_;
    Mesh quad_;
    Mesh worldMesh_;

    Material postProcessMaterial_;
    Material worldMaterial_;
    std::array<Material, 2> spreadMaterials_;
    Material seedMaterial_;
    Material textMaterial_;
    Material barMaterial_;
    Material blitMaterial_;

    Texture noiseTexture_;

    void InitQuad_P();
    void InitWorldMesh_P();

    void InitShadowBuffer_P();
    void InitRenderBuffer_P();
    void InitPostProcessBuffer_P();
    void InitUIBuffer_P();

    FrameBuffer backBuffer_;
    FrameBuffer shadowBuffer_;
    FrameBuffer renderBuffer_;
    FrameBuffer postProcessBuffer_;
    Texture shadowBufferTexture_;
    std::array<Texture, 2> renderBufferTextures_;
    Texture postProcessTexture_;

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

    Texture MakeNoiseTexture_P(Noise& noise);

    std::unordered_map<std::string, Shader> vertexShaders_;
    std::unordered_map<std::string, Shader> fragmentShaders_;
    std::unordered_map<std::string, Model> models_;
    std::unordered_map<std::string, Skeleton> skeletons_;
    std::unordered_map<std::string, Texture> textures_;
    std::unordered_map<std::string, Material> materials_;

    glm::mat4 GetModelViewProjection(const glm::mat4& modelMatrix);

    void SetTexturesFromMaterial_P(Material& material, bool shadowMap = true);

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

    void SetLightDirection_P(glm::vec3 direction);
};

