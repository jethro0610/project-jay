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
    Renderer(GLFWwindow* window);
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

    void LoadVertexShader_P(std::string name);
    void LoadFragmentShader_P(std::string name);
    void LoadModel_P(std::string name);
    void LoadTexture_P(std::string name);
    void MakeMaterial_P(
        std::string name, 
        std::string vertex, 
        std::string fragment, 
        std::string textures[MAX_TEXTURES_PER_MATERIAL], 
        uint8_t numTextures
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

    glm::mat4 viewMatrix_;
    glm::mat4 projectionMatrix_;

    TextureSampler samplers_[MAX_TEXTURES_PER_MATERIAL];

    Uniform u_normal_;
    Uniform u_timeResolution_;
    Uniform u_cameraPosition_;
    Uniform u_cameraUp_;
    Uniform u_cameraRight_;

    Mesh worldMesh_;
    Material worldMaterial_;
    Texture noiseTexture_;

    std::unordered_map<std::string, Shader> vertexShaders_;
    std::unordered_map<std::string, Shader> fragmentShaders_;
    std::unordered_map<std::string, Model> models_;
    std::unordered_map<std::string, Texture> textures_;
    std::unordered_map<std::string, Material> materials_;

    glm::mat4 GetWorldViewProjection(glm::mat4 worldMatrix);

    // void SetMaterial_P(std::string materialName);
    // void EnableBlend_P();
    // void DisableBlend_P();
    // void SetRenderTargetWorld_P();
    // void SetRenderTargetScreen_P();

    void StartFrame_P();
    void Clear_P();
    void RenderWorld_P(World& world);
    void RenderEntities_P(
        Entity* entities, 
        StaticModelComponent& staticModelComponent,
        TransformComponent& transformComponent
    );
    void RenderSpread_P(SpreadManager& spreadManager);
    void RenderSeed_P(SeedManager& seedManager);
    void RenderPostProcess_P();
    void RenderUI_P(MeterComponent& meterComponent);
    #ifdef _DEBUG
    void RenderScreenText_P();
    #endif
    void PresentFrame_P();
};

