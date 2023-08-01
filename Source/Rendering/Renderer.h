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
    Material MakeMaterial_P(
        std::string name, 
        Shader vertex, 
        Shader fragment, 
        Texture textures[MAX_TEXTURES_PER_MATERIAL], 
        int numTextures,
        bool twoSided = false
    );
    Material MakeMaterial_P(
        std::string name, 
        std::string vertex, 
        std::string fragment, 
        std::string textures[MAX_TEXTURES_PER_MATERIAL], 
        int numTextures,
        bool twoSided = false
    );

    void TEMP_LoadTestData();

    Shader GetVertexShader(std::string name);
    Shader GetFragmentShader(std::string name);
    Model GetModel(std::string name);
    Texture GetTexture(std::string name);
    Material GetMaterial(std::string name);

private:
    int width_;
    int height_;

    glm::vec3 lightDirection_;
    glm::mat4 viewMatrix_;
    glm::mat4 projectionMatrix_;
    glm::mat4 shadowViewMatrix_;
    glm::mat4 shadowProjectionMatrix_;
    glm::mat4 shadowMatrix_;
    // Shader shadowFS_;

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

    View shadowView_;
    View renderView_;
    View postProcessView_;
    View uiView_;

    FrameBuffer backBuffer_;
    FrameBuffer shadowBuffer_;
    FrameBuffer renderBuffer_;
    FrameBuffer postProcessBuffer_;
    Texture shadowBufferTexture_;
    Texture renderBufferTextures_[2];
    Texture postProcessTexture_;

    TextureSampler samplers_[MAX_TEXTURES_PER_MATERIAL];
    TextureSampler shadowSampler_;

    Uniform u_shadowMatrix_;
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

    void SetTexturesFromMaterial_P(View view, Material& material);

    void StartFrame_P();
    void Clear_P();
    void RenderWorld_P(View view, World& world);
    void RenderEntities_P(
        View view,
        Entity* entities, 
        MeterComponent& meterComponent,
        StaticModelComponent& staticModelComponent,
        TransformComponent& transformComponent
    );
    void RenderSpread_P(View view, SpreadManager& spreadManager);
    void RenderSeed_P(View view, SeedManager& seedManager);
    void RenderPostProcess_P(View view);
    void RenderBlit_P(View view);
    void RenderUI_P(View view, MeterComponent& meterComponent);
    #ifdef _DEBUG
    void RenderScreenText_P(View view);
    #endif
    void PresentFrame_P();

    void SetLightDirection_P(glm::vec3 direction);
};

