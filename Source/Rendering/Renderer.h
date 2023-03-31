#pragma once

#include "../Resource/ResourceManager.h"
#include "RenderTypes.h"

#include "../Game/Entity/Entity.h"
#include "../Game/Components/StaticModelComponent.h"
#include "../Game/Components/TransformComponent.h"
#include "../Game/Camera.h"
#include "../Game/SpreadManager.h"
#include "../Logging/ScreenText.h"

typedef std::unordered_map<std::string, std::vector<int>> StaticModelRenderList;

struct RenderComponents {
    StaticModelComponent& staticMeshComponents;
    TransformComponent& transformComponents;
};

class Renderer {
public:
    Renderer(ResourceManager& resourceManager);
    Camera* camera_;

    int width_;
    int height_;

    mat4 viewMatrix_;
    mat4 projMatrix_;
    void UpdateViewMatrix();
    void UpdateProjMatrix(float fov, float nearClip, float farClip);

    mat4 GetWorldViewProjection(mat4 worldMatrix);
    void Render(float deltaTime, float elapsedTime, Entity* entities, RenderComponents renderComponents, SpreadManager& spreadManager);
private:
    ResourceManager& resourceManager_;
	
    void SetFrameData_P();
    void Clear_P();
    void SetMaterial_P(std::string materialName);
    void RenderWorld_P();
    void RenderEntities_P(Entity* entities, RenderComponents renderComponents);
    void RenderSpread_P(SpreadManager& spreadManager);
    void RenderPostProcess_P();
    #ifdef _DEBUG
    void RenderScreenText_P();
    #endif
    void RenderTest();
    void Present_P();
};

