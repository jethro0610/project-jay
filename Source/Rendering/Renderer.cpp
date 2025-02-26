#include "Renderer.h"
#include "Camera/Camera.h"
#include "Level/LevelProperties.h"
#include "Editor/Editor.h"
#include "Terrain/Terrain.h"
#include "Entity/EntityList.h"
#include "Entity/Entity.h"
using namespace glm;

mat4 Renderer::GetModelViewProjection(const mat4& modelMatrix) {
    return projectionMatrix_ * camera_->GetViewMatrix() * modelMatrix;
}

void Renderer::Render(
    Clock& clock,
    EntityList& entities,
    LevelProperties& levelProperties,
    ParticleManager& particleManager, 
    SeedManager& seedManager,
    SpreadManager& spreadManager,
    Terrain& terrain
) {
    StartFrame();

    RenderTerrain(terrain, &levelProperties.terrainMaterial, TerrainConsts::RANGE);
    RenderEntities(entities);
    RenderSpread(spreadManager, levelProperties.spreadModel, levelProperties.spreadMaterials);
    RenderSeed(seedManager, &levelProperties.seedMaterial);
    RenderParticles(particleManager);
    RenderPostProcess();
    RenderBlit();
    RenderUI(clock);
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
    RenderStaticTerrainModifiers(terrain);
    RenderEntityEditorNodes(entities);
    RenderEntities(entities);
    RenderSpread(spreadManager, levelProperties.spreadModel, levelProperties.spreadMaterials);
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
