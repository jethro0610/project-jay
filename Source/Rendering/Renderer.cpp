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

    // Shadow Pass
    RenderEntities_P(shadowView_, entities, meterComponent, staticModelComponent, transformComponent);
    RenderWorld_P(shadowView_, world);
    RenderSpread_P(shadowView_, spreadManager);
    RenderSeed_P(shadowView_, seedManager);

    // Render Pass
    RenderEntities_P(renderView_, entities, meterComponent, staticModelComponent, transformComponent);
    RenderWorld_P(renderView_, world);
    RenderSpread_P(renderView_, spreadManager);
    RenderSeed_P(renderView_, seedManager);

    // Post Process Pass
    RenderPostProcess_P(postProcessView_);

    // UI Pass
    RenderBlit_P(uiView_);
    RenderUI_P(uiView_, meterComponent);
    RenderScreenText_P(uiView_);

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

Texture Renderer::GetTexture(std::string name) {
    return GetFromMap<Texture>(textures_, name, "Tried using unloaded texture " + name);
}

Material Renderer::GetMaterial(std::string name) {
    return GetFromMap<Material>(materials_, name, "Tried using unloaded material " + name);
}
