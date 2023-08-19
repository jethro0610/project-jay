#include "Renderer.h"
#include "../Game/Camera.h"
#include "../Helpers/MapCheck.h"
using namespace glm;

mat4 Renderer::GetWorldViewProjection(mat4 worldMatrix) {
    return projectionMatrix_ * camera_->GetViewMatrix() * worldMatrix;
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
    StartFrame_P();

    RenderWorld_P(world);
    RenderEntities_P(entities, meterComponent, staticModelComponent, transformComponent);
    RenderSpread_P(spreadManager);
    RenderSeed_P(seedManager);

    RenderPostProcess_P();

    RenderBlit_P();
    RenderUI_P(meterComponent);
    RenderScreenText_P();

    PresentFrame_P();
}

Shader Renderer::GetVertexShader(std::string name) {
    return GetFromMap<Shader>(vertexShaders_, name, "Tried using unloaded vertex shader " + name);
}

Shader Renderer::GetFragmentShader(std::string name) {
    return GetFromMap<Shader>(fragmentShaders_, name, "Tried using unloaded fragment shader " + name);
}

Model Renderer::GetModel(std::string name) {
    return GetFromMap<Model>(models_, name, "Tried using unloaded model " + name);
}

Skeleton Renderer::GetSkeleton(std::string name) {
    return &GetFromMap<Skeleton_INTERNAL>(skeletons_, name, "Tried using unloaded skeleton " + name);
}

Texture Renderer::GetTexture(std::string name) {
    return GetFromMap<Texture>(textures_, name, "Tried using unloaded texture " + name);
}

Material Renderer::GetMaterial(std::string name) {
    return GetFromMap<Material>(materials_, name, "Tried using unloaded material " + name);
}
