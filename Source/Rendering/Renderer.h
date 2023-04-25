#pragma once
#include <glm/mat4x4.hpp>
#include "RenderTypes.h"
class Camera;
class Entity;
class PlayerController;
class ResourceManager;
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
        SpreadManager& spreadManager,
        World& world,
        MeterComponent& meterComponent,
        StaticModelComponent& staticModelComponent,
        TransformComponent& transformComponent,
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
        StaticModelComponent& staticModelComponent,
        TransformComponent& transformComponent
    );
    void RenderSpread_P(SpreadManager& spreadManager);
    void RenderPostProcess_P();
    void RenderUI_P(MeterComponent& meterComponent);
    #ifdef _DEBUG
    void RenderScreenText_P();
    #endif
    void Present_P();
};

