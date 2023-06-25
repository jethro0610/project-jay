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
// class ResourceManager;
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

    int width_;
    int height_;

    glm::mat4 viewMatrix_;
    glm::mat4 projectionMatrix_;
    void InitProjMatrix(float fov, float nearClip, float farClip);

    TextureSampler samplers_[8];

    Uniform normalU_;
    Uniform timeResolutionU_;
    Uniform cameraPositionU_;
    Uniform cameraUpU_;
    Uniform cameraRightU_;

    std::unordered_map<std::string, Shader> vertexShaders_;
    std::unordered_map<std::string, Shader> fragmentShaders_;
    std::unordered_map<std::string, Model> models_;
    std::unordered_map<std::string, Texture> textures_;
    std::unordered_map<std::string, Material> materials_;

    glm::mat4 GetWorldViewProjection(glm::mat4 worldMatrix);
    void Render(
        Entity* entities,
        SeedManager& seedManager,
        SpreadManager& spreadManager,
        World& world,
        MeterComponent& meterComponent,
        StaticModelComponent& staticModelComponent,
        TransformComponent& transformComponent
    );

    bool LoadVertexShader_P(std::string name);
    bool LoadFragmentShader_P(std::string name);
    bool LoadModel_P(std::string name);
    bool LoadTexture_P(std::string name);

    void TEMP_LoadTestData();

private:
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

