#pragma once
#ifdef _PC
#include <GLFW/glfw3.h>
#endif

#include <glm/mat4x4.hpp>
#include <string>
#include <unordered_map>
#include "RenderDefs.h"
#include "Model.h"
#include "Material.h"

class Camera;
class Entity;
class FastNoiseLite;
class PlayerController;
class SeedManager;
class SpreadManager;
class World;
class MeterComponent;
class StaticModelComponent;
class TransformComponent;

#ifdef _DEBUG
#include "../Logging/ScreenText.h"
#endif

class Renderer {
public:
    #ifdef _PC
    Renderer(FastNoiseLite& noise, GLFWwindow* window);
    #endif
    Camera* camera_;

    void Render(
        Entity* entities,
        SeedManager& seedManager,
        SpreadManager& spreadManager,
        World& world,
        MeterComponent& meterComponent,
        StaticModelComponent& staticModelComponent,
        TransformComponent& transformComponent
    );

    Shader LoadVertexShader_P(std::string name);
    Shader LoadFragmentShader_P(std::string name);
    Model LoadModel_P(std::string name);
    Texture LoadTexture_P(std::string name);
    Material LoadMaterial_P(std::string name);

    void TEMP_LoadTestData();

    Shader GetVertexShader(std::string name);
    Shader GetFragmentShader(std::string name);
    Model GetModel(std::string name);
    Texture GetTexture(std::string name);
    Material GetMaterial(std::string name);

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
    Material spreadMaterials_[MAX_MESHES_PER_MODEL];
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
    Texture renderBufferTextures_[2];
    Texture postProcessTexture_;

    TextureSampler samplers_[NUM_SAMPLERS];

    Uniform u_shadowUp_;
    Uniform u_shadowRight_;
    Uniform u_shadowMatrix_;
    Uniform u_shadowResolution_;

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

    Texture MakeNoiseTexture_P(FastNoiseLite& noise, int resolution, float distance);

    std::unordered_map<std::string, Shader> vertexShaders_;
    std::unordered_map<std::string, Shader> fragmentShaders_;
    std::unordered_map<std::string, Model> models_;
    std::unordered_map<std::string, Texture> textures_;
    std::unordered_map<std::string, Material> materials_;

    glm::mat4 GetWorldViewProjection(glm::mat4 worldMatrix);

    void SetTexturesFromMaterial_P(Material& material, bool shadowMap = true);

    void StartFrame_P();
    void Clear_P();
    void RenderMesh_P(Mesh* mesh, Material* material, InstanceBuffer* instanceBuffer = nullptr, glm::mat4* worldMatrix = nullptr);
    void RenderWorld_P(World& world);
    void RenderEntities_P(
        Entity* entities, 
        MeterComponent& meterComponent,
        StaticModelComponent& staticModelComponent,
        TransformComponent& transformComponent
    );
    void RenderSpread_P(SpreadManager& spreadManager);
    void RenderSeed_P(SeedManager& seedManager);
    void RenderPostProcess_P();
    void RenderBlit_P();
    void RenderUI_P(MeterComponent& meterComponent);
    #ifdef _DEBUG
    void RenderScreenText_P();
    #endif
    void PresentFrame_P();

    void SetLightDirection_P(glm::vec3 direction);
};

