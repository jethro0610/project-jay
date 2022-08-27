#pragma once

#include "../Resource/ResourceManager.h"
#include "../Types/Transform.h"
#include "RenderTypes.h"

#include "../Game/Components/StaticModelComponent.h"
#include "../Game/Components/TransformComponent.h"
#include "../Game/Camera.h"

typedef std::unordered_map<std::string, std::vector<int>> StaticModelRenderList;

struct RenderComponents {
    StaticModelComponent& staticMeshComponents;
    TransformComponent& transformComponents;
};

class Renderer {
public:
    Renderer(ResourceManager* resourceManager);
    Camera* camera_;
    void Init_P();

    int width_;
    int height_;

    mat4 viewMatrix_;
    mat4 projMatrix_;
    void UpdateViewMatrix();
    void UpdateProjMatrix(float fov, float nearClip, float farClip);

    mat4 GetWorldViewProjection(mat4 worldMatrix);

    void BuildStaticModelRenderList (
        RenderComponents renderComponents,
        StaticModelRenderList& outStaticMeshRenderList
    );
    void Render(float deltaTime, float elapsedTime, RenderComponents renderComponents);

private:
    ResourceManager* resourceManager_;

    void Clear_P();
    void RenderWorld_P();
    void RenderStaticMeshes_P(RenderComponents renderComponents, const StaticModelRenderList& staticModelRenderList);
    void Present_P();
};

