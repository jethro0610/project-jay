#include "Renderer.h"
#include "Camera/Camera.h"
#include "Level/LevelProperties.h"
using namespace glm;

mat4 Renderer::GetModelViewProjection(const mat4& modelMatrix) {
    return projectionMatrix_ * camera_->GetViewMatrix() * modelMatrix;
}

void Renderer::Render(
    EntityList& entities,
    ComponentList& components,
    LevelProperties& levelProperties,
    ParticleManager& particleManager, 
    SeedManager& seedManager,
    SpreadManager& spreadManager,
    Terrain& terrain 
) {
    StartFrame();

    RenderTerrain(terrain, levelProperties.terrainMaterial);
    RenderEntities(entities, components);
    RenderSpread(spreadManager, levelProperties.spreadModel, levelProperties.spreadMaterials);
    RenderSeed(seedManager, levelProperties.seedMaterial);
    RenderParticles(particleManager);
    RenderPostProcess();
    RenderBlit();
    RenderUI(components);
    RenderScreenText();

    PresentFrame();
}

void Renderer::RenderEdit(
    EntityList& entities,
    ComponentList& components,
    Editor& editor,
    LevelProperties& levelProperties,
    Terrain& terrain 
) {
    StartFrame();

    RenderTerrain(terrain, levelProperties.terrainMaterial);
    RenderEntities(entities, components);
    RenderPostProcess();
    RenderBlit();
    RenderEditor(editor);

    PresentFrame();
}
