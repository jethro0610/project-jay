#include "Renderer.h"
#include <fstream>
#include <GLFW/glfw3native.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "../Game/Camera.h"
#include "../Helpers/EntityHelpers.h"
#include "../Logging/Logger.h"
#include "../Game/Time.h"
#include "../Resource/RawModel.h"

#include "../Game/Entity/Entity.h"
#include "../Game/Components/StaticModelComponent.h"
#include "../Game/Components/TransformComponent.h"

#include "PC_VertexTypes.h"

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

    InitProjMatrix(70.0f, 0.5f, 1000.0f);

    for (int i = 0; i < 8; i++) {
        std::string samplerName = "s_sampler" + std::to_string(i);
        samplers_[i] = bgfx::createUniform(samplerName.c_str(), bgfx::UniformType::Sampler);
    }

    normalU_ = bgfx::createUniform("u_normal", bgfx::UniformType::Mat4);
    timeResolutionU_ = bgfx::createUniform("u_timeResolution", bgfx::UniformType::Vec4);
    cameraPositionU_ = bgfx::createUniform("u_cameraPosition", bgfx::UniformType::Vec4);
    cameraUpU_ = bgfx::createUniform("u_cameraUp", bgfx::UniformType::Vec4);
    cameraRightU_ = bgfx::createUniform("u_cameraRight", bgfx::UniformType::Vec4);
}

void Renderer::StartFrame_P() {
    mat4 viewMatrix = camera_->GetViewMatrix();
    bgfx::setViewTransform(0, &viewMatrix ,&projectionMatrix_);

    vec4 timeResolution = vec4(Time::GetTime(), 1280, 720, 0.0f);
    vec4 cameraPosition = vec4(camera_->transform_.position_, 0.0f);
    vec4 cameraUp = vec4(camera_->transform_.GetUpVector(), 0.0f);
    vec4 cameraRight = vec4(camera_->transform_.GetRightVector(), 0.0f);

    bgfx::setUniform(timeResolutionU_, &timeResolution);
    bgfx::setUniform(cameraPositionU_, &cameraPosition);
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

EntityKey constexpr key = GetEntityKey<StaticModelComponent, TransformComponent>();
void Renderer::RenderEntities_P(
    Entity* entities, 
    StaticModelComponent& staticModelComponent,
    TransformComponent& transformComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;
        if (!entity.MatchesKey(key))
            continue;

        auto [worldMatrix, normalMatrix] = transformComponent.renderTransform[i].GetWorldAndNormalMatrix();
        bgfx::setTransform(&worldMatrix);
        bgfx::setUniform(normalU_, &normalMatrix);

        Model model = models_[staticModelComponent.model[i]];
        for (int m = 0; m < model.numMeshes; m++) {
            Material material = materials_[staticModelComponent.materials[i][m]];
            Mesh mesh = model.meshes[m];

            for (int t = 0; t < material.numTextures; i++)
                bgfx::setTexture(t, samplers_[t], material.textures[t]);

            bgfx::setVertexBuffer(0, mesh.vertexBuffer);
            bgfx::setIndexBuffer(mesh.indexBuffer);
            bgfx::submit(0, material.shader);
        }
    }
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

const bgfx::Memory* LoadFileToBGFXMem(std::string path) {
    std::ifstream file;
    file.open(path);
    if (!file.is_open()) {
        DEBUGLOG("Failed to open bgfx file: " + path);
        return nullptr;
    }
    
    file.seekg(0, file.end);
    size_t fileSize = file.tellg();
    file.seekg(0, file.beg); 

    const bgfx::Memory* memory = bgfx::alloc(fileSize + 1);
    file.read((char*)memory->data, fileSize);
    file.close();

    return memory;
}

bool Renderer::LoadVertexShader_P(std::string name) {
    const bgfx::Memory* shaderMem = LoadFileToBGFXMem("./shaders/" + name + ".bin");
    if (shaderMem == nullptr)
        return false;

    vertexShaders_[name] = bgfx::createShader(shaderMem);
    return true;
}

bool Renderer::LoadFragmentShader_P(std::string name) {
    const bgfx::Memory* shaderMem = LoadFileToBGFXMem("./shaders/" + name + ".bin");
    if (shaderMem == nullptr)
        return false;

    fragmentShaders_[name] = bgfx::createShader(shaderMem);
    return true;
}

bool Renderer::LoadModel_P(std::string name) {
    Model model;
    RawModel rawModel("./" + name + ".jmd", false);
    model.numMeshes = rawModel.meshes_.size();

    for (int i = 0; i < rawModel.meshes_.size(); i++) {
        RawMesh& rawMesh = rawModel.meshes_[i];
        Mesh mesh;
        mesh.vertexBuffer = bgfx::createVertexBuffer(
            bgfx::makeRef(
                rawMesh.vertexBuffer_, 
                rawMesh.vertexCount_ * sizeof(StaticVertex)
            ), 
            StaticVertex::layout
        );
        mesh.vertexCount = rawMesh.vertexCount_;

        mesh.indexBuffer = bgfx::createIndexBuffer(
            bgfx::makeRef(
                rawModel.meshes_[i].indexBuffer_,
                rawMesh.indexCount_ * sizeof(uint16_t)
            )
        );
        mesh.indexCount = rawMesh.indexCount_;
        model.meshes[i] = mesh;
    }

    return true;
}

bool Renderer::LoadTexture_P(std::string name) {
    const bgfx::Memory* textureMem = LoadFileToBGFXMem("./textures/" + name + ".tex"); 
    if (textureMem == nullptr)
        return false;

    textures_[name] = bgfx::createTexture(textureMem);
    return true;
}
