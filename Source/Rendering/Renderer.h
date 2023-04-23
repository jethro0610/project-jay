#pragma once
#include <glm.hpp>
#include "RenderTypes.h"
class Camera;
class Entity;
class PlayerController;
class ResourceManager;
class SpreadManager;
class World;
class TransformComponent;
class StaticModelComponent;

#ifdef _DEBUG
#include "../Logging/ScreenText.h"
#endif

class Renderer {
public:
    Renderer(ResourceManager& resourceManager);
    Camera* camera_;

    int width_;
    int height_;

    glm::mat4 viewMatrix_;
    glm::mat4 projMatrix_;
    void UpdateViewMatrix();
    void UpdateProjMatrix(float fov, float nearClip, float farClip);

    glm::mat4 GetWorldViewProjection(glm::mat4 worldMatrix);
    void Render(
        Entity* entities,
        PlayerController& playerController,
        SpreadManager& spreadManager,
        World& world,
        TransformComponent& transformComponent,
        StaticModelComponent& staticModelComponent,
        float deltaTime, 
        float elapsedTime
    );
private:
    ResourceManager& resourceManager_;
	
    void SetFrameData_P();
    void Clear_P();
    void SetMaterial_P(std::string materialName);
    void RenderWorld_P(World& world);
    void RenderEntities_P(
        Entity* entities, 
        TransformComponent& transformComponent, 
        StaticModelComponent& staticModelComponent
    );
    void RenderSpread_P(SpreadManager& spreadManager);
    void RenderPostProcess_P();
    void RenderUI_P(PlayerController& playerController);
    #ifdef _DEBUG
    void RenderScreenText_P();
    #endif
    void Present_P();
};

