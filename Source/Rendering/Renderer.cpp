#include "Renderer.h"
#include "Camera/Camera.h"
#include "Level/LevelProperties.h"
#include "Entity/Player.h"
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
    Terrain& terrain,
    Player& player
) {
    StartFrame();

    RenderTerrain(terrain, levelProperties.terrainMaterial, levelProperties.blob.maxRadius);
    RenderEntities(entities, components);
    glm::mat4 matrix = player.transform_.ToMatrix();
    RenderMesh(&player.model_->meshes[0], player.material_, nullptr, &matrix, nullptr);
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

    RenderTerrain(terrain, levelProperties.terrainMaterial, levelProperties.blob.maxRadius);
    RenderEntities(entities, components, true);
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
