#include "Renderer.h"
#include <GLFW/glfw3native.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "../Game/Camera.h"
#include "../Game/Time.h"

using namespace glm;

Renderer::Renderer(GLFWwindow* window) {
    bgfx::Init init; 
    init.type = bgfx::RendererType::Count;
    init.resolution.width = 1280;
    init.resolution.height = 720;
    init.resolution.reset = BGFX_RESET_VSYNC;
    init.platformData.nwh = GETHANDLE(window);
    bgfx::init(init);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, 1280, 720);

    worldViewProjU_ = bgfx::createUniform("u_worldViewProj", bgfx::UniformType::Mat4);
    timeResolutionU_ = bgfx::createUniform("u_timeResolution", bgfx::UniformType::Vec4);
    cameraPosU_ = bgfx::createUniform("u_cameraPos", bgfx::UniformType::Vec4);
    cameraUpU_ = bgfx::createUniform("u_cameraUp", bgfx::UniformType::Vec4);
    cameraRightU_ = bgfx::createUniform("u_cameraRight", bgfx::UniformType::Vec4);
}

void Renderer::StartFrame_P() {
    vec4 timeResolution = vec4(Time::GetTime(), 1280, 720, 0.0f);
    vec4 cameraPos = vec4(camera_->transform_.position_, 0.0f);
    vec4 cameraUp = vec4(camera_->transform_.GetUpVector(), 0.0f);
    vec4 cameraRight = vec4(camera_->transform_.GetRightVector(), 0.0f);

    bgfx::setUniform(timeResolutionU_, &timeResolution);
    bgfx::setUniform(cameraPosU_, &cameraPos);
    bgfx::setUniform(cameraUpU_, &cameraUp);
    bgfx::setUniform(cameraRightU_, &cameraRight);
}

void Renderer::Clear_P() {
    return;
}

void Renderer::PresentFrame_P() {
    bgfx::touch(0);
    bgfx::frame(); 
}

void RenderWorld_P(World& world) {
    // Take the world vertices and present them
}

void RenderEntities_P(
    Entity* entities, 
    StaticModelComponent& staticModelComponent,
    TransformComponent& transformComponent
) {
    // Take the entity models and present them
}

void RenderSpread_P(SpreadManager& spreadManager) {
    // Take the spread buffers and present them (instanced)
}

void RenderSeed_P(SeedManager& seedManager) {
    // Take the seed buffers and present them (instanced)
}

void RenderPostProcess_P() {
    // Kuwahara
}

void RenderUI_P(MeterComponent& meterComponent) {
    // Create plane and present
}

void RenderScreenText_P() {
    // Create plane and present (instanced)
}
