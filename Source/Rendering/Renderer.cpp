#include "Renderer.h"
#include "../Game/Camera.h"
using namespace glm;

void Renderer::InitProjMatrix(float fov, float nearClip, float farClip) {
    projMatrix_ = perspectiveFovRH_ZO(radians(fov), (float)width_, (float)height_, nearClip, farClip);
}

mat4 Renderer::GetWorldViewProjection(mat4 worldMatrix) {
    return projMatrix_ * camera_->GetViewMatrix() * worldMatrix;
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
    Clear_P();
    StartFrame_P();

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
    PresentFrame_P();
}
