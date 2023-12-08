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
    StartFrame();

    RenderWorld(world);
    RenderEntities(entities, components);
    RenderSpread(spreadManager);
    RenderSeed(seedManager);
    RenderParticles(particleManager);
    RenderPostProcess();
    RenderBlit();
    RenderUI(components);
    RenderScreenText();

    PresentFrame();
}
