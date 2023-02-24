#include "Renderer.h"

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

void Renderer::Render(float deltaTime, float elapsedTime, Entity* entities, RenderComponents renderComponents, SpreadManager* spreadManager) {
    UpdateViewMatrix();
    Clear_P();
    SetFrameData_P();
    RenderWorld_P();
    StaticModelRenderList staticModelRenderList;
    RenderEntities_P(entities, renderComponents);
    RenderSpread_P(spreadManager);
    Present_P();
}
