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
    EntityList& entities,
    LevelController& levelController,
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
    RenderComposite();
    RenderPostProcess();
    RenderBlit();
    RenderUI(entities, levelController);
    RenderScreenText();

    PresentFrame();
}

#ifdef _DEBUG
void Renderer::RenderEdit(
    EntityList& entities,
    Editor& editor,
    Level& level,
    LevelController& controller,
    LevelProperties& levelProperties,
    SpreadManager& spreadManager,
    Terrain& terrain,
    TerrainCursor& terrainCursor
) {
    DBG_lowResTerrain_ = terrain.DBG_lowRes_;
    StartFrame();

    RenderTerrain(terrain, &levelProperties.terrainMaterial, TerrainConsts::RANGE);
    RenderStaticTerrainModifiers(terrain);
    RenderEditorNodes(level);
    RenderEntities(entities);
    RenderSpread(spreadManager, levelProperties.spreadModel, levelProperties.spreadMaterials);
    RenderTerrainCursor(terrainCursor);
    RenderComposite();
    RenderPostProcess();
    RenderBlit();
    RenderEditor(editor);
    RenderSeedTexts(entities, controller);

    PresentFrame();
}

void Renderer::RenderEditorOnly(Editor& editor) {
    StartFrame();
    RenderEditor(editor);
    PresentFrame();
}
#endif
