#include "Renderer.h"
#include "Camera/Camera.h"
#include "Level/LevelProperties.h"
using namespace glm;

mat4 Renderer::GetModelViewProjection(const mat4& modelMatrix) {
    return projectionMatrix_ * camera_->GetViewMatrix() * modelMatrix;
}

void Renderer::Render(
    EntityList& entities,
    LevelProperties& levelProperties,
    ParticleManager& particleManager, 
    SeedManager& seedManager,
    SpreadManager& spreadManager,
    Terrain& terrain
) {
    StartFrame();

    RenderTerrain(terrain, &levelProperties.terrainMaterial, 500.0f);
    RenderEntitiesS(entities);
    RenderSpread(spreadManager, levelProperties.spreadModel, levelProperties.spreadMaterials);
    RenderSeed(seedManager, &levelProperties.seedMaterial);
    RenderParticles(particleManager);
    RenderPostProcess();
    RenderBlit();
    RenderUI(entities);
    RenderScreenText();

    PresentFrame();
}

#ifdef _DEBUG
void Renderer::RenderEdit(
    EntityList& entities,
    Editor& editor,
    LevelProperties& levelProperties,
    Terrain& terrain 
) {
    StartFrame();

    RenderTerrain(terrain, &levelProperties.terrainMaterial, 500.0f);
    RenderTerrainBubbles(terrain);
    RenderTerrainCurves(terrain);
    RenderEntitiesS(entities);
    RenderPostProcess();
    RenderBlit();
    RenderEditor(editor);

    PresentFrame();
}

void Renderer::RenderEditorOnly(Editor& editor) {
    StartFrame();
    RenderEditor(editor);
    PresentFrame();
}
#endif
