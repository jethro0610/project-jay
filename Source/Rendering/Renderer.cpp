#include "Renderer.h"
#include "Camera/Camera.h"
#include "Level/LevelProperties.h"
#include "Editor/Editor.h"
#include "Terrain/Terrain.h"
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

    RenderTerrain(terrain, &levelProperties.terrainMaterial, TerrainConsts::RANGE);
    RenderEntitiesS(entities);
    RenderSpread(spreadManager, levelProperties.spreadModel, levelProperties.spreadMaterials);
    RenderWeed(spreadManager, levelProperties.weedModel_, levelProperties.weedMaterials);
    RenderSeed(seedManager, &levelProperties.seedMaterial);
    RenderParticles(particleManager);
    RenderPostProcess();
    RenderBlit();
    RenderUI(entities, spreadManager);
    RenderScreenText();

    PresentFrame();
}

#ifdef _DEBUG
void Renderer::RenderEdit(
    EntityList& entities,
    Editor& editor,
    LevelProperties& levelProperties,
    SpreadManager& spreadManager,
    Terrain& terrain,
    TerrainCursor& terrainCursor
) {
    DBG_lowResTerrain_ = terrain.DBG_lowRes_;
    StartFrame();

    RenderTerrain(terrain, &levelProperties.terrainMaterial, TerrainConsts::RANGE);
    if (editor.visibility_ == EV_All || editor.visibility_ == EV_TerrainControlsOnly) {
        RenderTerrainBubbles(terrain);
        RenderTerrainCurves(terrain);
    }
    if (editor.visibility_ == EV_All || editor.visibility_ == EV_EntitiesOnly)
        RenderEntitiesS(entities);
    RenderSpread(spreadManager, levelProperties.spreadModel, levelProperties.spreadMaterials);
    RenderWeed(spreadManager, levelProperties.weedModel_, levelProperties.weedMaterials);
    RenderTerrainCursor(terrainCursor);
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
