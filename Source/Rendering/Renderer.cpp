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
    glm::mat4 matrix = player.renderTransform_.ToMatrix();
    GPUPose gpuPose;
    player.skeleton_->PoseToGPUPose(
        gpuPose,
        player.renderPose_
    );
    for (int i = 0; i < 8; i++)
        RenderMesh(&player.model_->meshes[i], player.materials_[i], nullptr, &matrix, &gpuPose);
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
