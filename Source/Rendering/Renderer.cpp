#include "Renderer.h"
#include "../Game/Camera.h"
using namespace glm;

void Renderer::UpdateViewMatrix() {
    assert(camera_ != nullptr);
    viewMatrix_ = camera_->GetViewMatrix();
}

void Renderer::UpdateProjMatrix(float fov, float nearClip, float farClip) {
    projMatrix_ = perspectiveFovRH_ZO(radians(fov), (float)width_, (float)height_, nearClip, farClip);
}

mat4 Renderer::GetWorldViewProjection(mat4 worldMatrix) {
    return projMatrix_ * viewMatrix_ * worldMatrix;
}

void Renderer::Render(
    Entity* entities,
    SpreadManager& spreadManager,
    World& world,
    TransformComponent& transformComponent,
    StaticModelComponent& staticModelComponent,
    float deltaTime, 
    float elapsedTime
) {
    UpdateViewMatrix();
    Clear_P();
    SetFrameData_P();
    RenderWorld_P(world);
    RenderEntities_P(entities, transformComponent, staticModelComponent);
    RenderSpread_P(spreadManager);
    RenderPostProcess_P();
    #ifdef _DEBUG
    RenderScreenText_P();
    #endif
    // RenderTextureToScreen_P();
    Present_P();
}
