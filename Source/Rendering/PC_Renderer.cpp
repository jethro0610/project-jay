#include "Renderer.h"
#include <fstream>
#include <GLFW/glfw3native.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>
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

bgfx::VertexLayout StaticVertex::layout;

Renderer::Renderer(GLFWwindow* window) {
    bgfx::Init init; 
    init.type = bgfx::RendererType::Count;
    init.resolution.width = 1280;
    init.resolution.height = 720;
    init.resolution.reset = BGFX_RESET_VSYNC;
    init.platformData.nwh = GETHANDLE(window);
    bgfx::init(init);

    width_ = 1280;
    height_ = 720;
    projectionMatrix_ = perspectiveFovRH_ZO(radians(70.0f), (float)width_, (float)height_, 0.5f, 1000.0f);

    for (int i = 0; i < 8; i++) {
        std::string samplerName = "s_sampler" + std::to_string(i);
        samplers_[i] = bgfx::createUniform(samplerName.c_str(), bgfx::UniformType::Sampler);
    }

    normalU_ = bgfx::createUniform("u_normal", bgfx::UniformType::Mat3);
    timeResolutionU_ = bgfx::createUniform("u_timeResolution", bgfx::UniformType::Vec4);
    cameraPositionU_ = bgfx::createUniform("u_cameraPosition", bgfx::UniformType::Vec4);
    cameraUpU_ = bgfx::createUniform("u_cameraUp", bgfx::UniformType::Vec4);
    cameraRightU_ = bgfx::createUniform("u_cameraRight", bgfx::UniformType::Vec4);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, 1280, 720);
    StaticVertex::Init();
    TEMP_LoadTestData();
}

void Renderer::TEMP_LoadTestData() {
    assert(LoadVertexShader_P("StaticVS"));
    assert(LoadFragmentShader_P("DefaultFS"));
    assert(LoadModel_P("st_sphere"));
    assert(LoadTexture_P("bricks_c"));
    Material playerMaterial;
    playerMaterial.shader = bgfx::createProgram(vertexShaders_["StaticVS"], fragmentShaders_["DefaultFS"]);
    playerMaterial.textures[0] = textures_["bricks_c"];
    playerMaterial.numTextures = 1;
    materials_["playerMaterial"] = playerMaterial;
}

void Renderer::StartFrame_P() {
    viewMatrix_ = camera_->GetViewMatrix();
    bgfx::setViewTransform(0, &viewMatrix_,&projectionMatrix_);

    vec4 timeResolution = vec4(Time::GetTime(), 1280, 720, 0.0f);
    vec4 cameraPosition = vec4(camera_->transform_.position_, 0.0f);
    vec4 cameraUp = vec4(camera_->transform_.GetUpVector(), 0.0f);
    vec4 cameraRight = vec4(camera_->transform_.GetRightVector(), 0.0f);

    bgfx::setUniform(timeResolutionU_, &timeResolution);
    bgfx::setUniform(cameraPositionU_, &cameraPosition);
    bgfx::setUniform(cameraUpU_, &cameraUp);
    bgfx::setUniform(cameraRightU_, &cameraRight);

    bgfx::touch(0);
}

void Renderer::Clear_P() {
    return;
}

void Renderer::PresentFrame_P() {
    bgfx::frame(); 
}

void Renderer::RenderWorld_P(World& world) {
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
            assert(materials_.contains("playerMaterial"));
            Material material = materials_[staticModelComponent.materials[i][m]];
            Mesh mesh = model.meshes[m];

            for (int t = 0; t < material.numTextures; t++)
                bgfx::setTexture(t, samplers_[t], material.textures[t]);

            bgfx::setVertexBuffer(0, mesh.vertexBuffer);
            bgfx::setIndexBuffer(mesh.indexBuffer);
            bgfx::submit(0, material.shader);
        }
    }
}

void Renderer::RenderSpread_P(SpreadManager& spreadManager) {
    // Take the spread buffers and present them (instanced)
}

void Renderer::RenderSeed_P(SeedManager& seedManager) {
    // Take the seed buffers and present them (instanced)
}

void Renderer::RenderPostProcess_P() {
    // Kuwahara
}

void Renderer::RenderUI_P(MeterComponent& meterComponent) {
    // Create plane and present
}

void Renderer::RenderScreenText_P() {
    // Create plane and present (instanced)
}

#define MEMORYFROMFILE(path)                                \
    const bgfx::Memory* memory = nullptr;                   \
    std::ifstream file;                                     \
    file.open(path, std::ios::binary);                      \
    if (!file.is_open())                                    \
        DEBUGLOG("Failed to open bgfx file: " + path);      \
                                                            \
    file.seekg(0, file.end);                                \
    size_t fileSize = file.tellg();                         \
    file.seekg(0, file.beg);                                \
                                                            \
    memory = bgfx::alloc(fileSize);                         \
    file.read((char*)memory->data, fileSize);               \
    file.close()


bool Renderer::LoadVertexShader_P(std::string name) {
    std::string path = "./shaders/" + name + ".bin";
    MEMORYFROMFILE(path);
    if (memory == nullptr)
        return false;

    vertexShaders_[name] = bgfx::createShader(memory);
    return true;
}

bool Renderer::LoadFragmentShader_P(std::string name) {
    std::string path = "./shaders/" + name + ".bin";
    MEMORYFROMFILE(path);
    if (memory == nullptr)
        return false;

    fragmentShaders_[name] = bgfx::createShader(memory);
    return true;
}

bool Renderer::LoadModel_P(std::string name) {
    Model model;
    std::ifstream file;
    file.open("./models/" + name + ".jmd", std::ios::in | std::ios::binary);
    if (!file.is_open())
        return false;

    ModelFileHeader modelHeader;
    file.read((char*)&modelHeader, sizeof(ModelFileHeader));
    model.numMeshes = modelHeader.numMeshes;
    std::cout << "Model has " << (int)model.numMeshes << " meshes\n";

    for (int i = 0; i < model.numMeshes; i++) {
        MeshFileHeader meshHeader;
        file.read((char*)&meshHeader, sizeof(MeshFileHeader));
        std::cout << "Mesh has " << meshHeader.numVertices << " vertices and " << meshHeader.numIndices << " indices\n";
        
        const bgfx::Memory* vertexMem = bgfx::alloc(sizeof(StaticVertex) * meshHeader.numVertices);
        file.read((char*)vertexMem->data, sizeof(StaticVertex) * meshHeader.numVertices);

        const bgfx::Memory* indexMem = bgfx::alloc(sizeof(uint16_t) * meshHeader.numIndices);
        file.read((char*)indexMem->data, sizeof(uint16_t) * meshHeader.numIndices);

        model.meshes[i].vertexBuffer = bgfx::createVertexBuffer(vertexMem, StaticVertex::layout);
        model.meshes[i].indexBuffer = bgfx::createIndexBuffer(indexMem);
    }
    models_[name] = model;

    return true;
}

bool Renderer::LoadTexture_P(std::string name) {
    std::string path = "./textures/" + name + ".dds";
    MEMORYFROMFILE(path);
    if (memory == nullptr)
        return false;

    textures_[name] = bgfx::createTexture(memory);
    return true;
}
