#pragma once

#include "../Resource/ResourceManager.h"
#include "../Types/Transform.h"
#include "RenderTypes.h"

#include "../Game/Components/ActiveEntityComponents.h"
#include "../Game/Components/StaticModelComponents.h"
#include "../Game/Components/TransformComponents.h"

typedef std::unordered_map<std::string, std::vector<int>> StaticModelRenderList;

struct RenderComponents {
    ActiveEntityComponents* activeEntityComponents;
    StaticModelComponents* staticMeshComponents;
    TransformComponents* transformComponents;
};

class Renderer {
public:
    Renderer(ResourceManager* resourceManager);
    void Init_P();

    ResourceManager* resourceManager_;

    int width_;
    int height_;

    mat4 viewMatrix_;
    mat4 projMatrix_;
    void UpdateViewMatrix();
    void UpdateProjMatrix(float fov, float nearClip, float farClip);

    mat4 GetWorldViewProjection(mat4 worldMatrix);

    Transform cameraTransform_;


    void BuildStaticModelRenderList (
        RenderComponents renderComponents,
        StaticModelRenderList& outStaticMeshRenderList
    );
    void Render(float deltaTime, float elapsedTime, RenderComponents renderComponents);

private:
    void Clear_P();
    void RenderWorld_P();
    void RenderStaticMeshes_P(RenderComponents renderComponents, const StaticModelRenderList& staticModelRenderList);
    void Present_P();
};

