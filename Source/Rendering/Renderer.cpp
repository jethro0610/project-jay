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
    SeedManager& seedManager,
    SpreadManager& spreadManager,
    World& world,
    MeterComponent& meterComponent,
    StaticModelComponent& staticModelComponent,
    TransformComponent& transformComponent
) {
    UpdateViewMatrix();
    Clear_P();
    SetFrameData_P();

    SetRenderTargetWorld_P();
    RenderWorld_P(world);
    RenderEntities_P(entities, staticModelComponent, transformComponent);
    RenderSpread_P(spreadManager);
    RenderSeed_P(seedManager);

    SetRenderTargetScreen_P();
    RenderPostProcess_P();
    RenderUI_P(meterComponent);
    #ifdef _DEBUG
    EnableBlend_P();
    RenderScreenText_P();
    DisableBlend_P();
    #endif

    // RenderTextureToScreen_P();
    Present_P();
}
