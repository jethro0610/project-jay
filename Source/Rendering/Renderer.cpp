#include "Renderer.h"
#include "../Game/Camera.h"
#include "../Helpers/MapCheck.h"
using namespace glm;

mat4 Renderer::GetModelViewProjection(const mat4& modelMatrix) {
    return projectionMatrix_ * camera_->GetViewMatrix() * modelMatrix;
}

void Renderer::Render(
    EntityList& entities,
    ComponentList& components,
    ParticleManager& particleManager, 
    SeedManager& seedManager,
    SpreadManager& spreadManager,
    World& world
) {
    StartFrame_P();

    RenderWorld_P(world);
    RenderEntities_P(entities, components);
    RenderSpread_P(spreadManager);
    RenderSeed_P(seedManager);
    RenderParticles_P(particleManager);
    RenderPostProcess_P();
    RenderBlit_P();
    RenderUI_P(components);
    RenderScreenText_P();

    PresentFrame_P();
}
