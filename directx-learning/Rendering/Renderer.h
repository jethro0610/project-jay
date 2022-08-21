#pragma once

#ifdef WINDOWS
#include "../Resource/DXResources.h"
#endif

#include "../Types/Transform.h"
#include "RenderTypes.h"

#include "../Game/Components/ActiveEntityComponents.h"
#include "../Game/Components/StaticMeshComponents.h"
#include "../Game/Components/TransformComponents.h"

class Renderer {
public:
    #ifdef WINDOWS
    Renderer(DXResources* dxLayer);
    DXResources* dxResources_;
    #endif
    void Init();
    ~Renderer();

    int width_;
    int height_;

    mat4 viewMatrix_;
    mat4 projMatrix_;
    void UpdateViewMatrix();
    void UpdateProjMatrix(float fov, float nearClip, float farClip);

    void Temp_GetWorldAndNormalMatrix(mat4& outWorld, mat4& outNormal);
    mat4 GetWorldViewProjection(mat4 worldMatrix);

    float testRot_;

    Transform cameraTransform_;

    void BuildStaticMeshRenderList (
        ActiveEntityComponents& activeEntityComponents,
        StaticMeshComponents& staticMeshComponents,
        TransformComponents& transformComponents
    );
    void Render();
    void Render_P();

private:
    void RenderWorld_P();
};

