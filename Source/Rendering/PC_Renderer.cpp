#include "Renderer.h"
#include <fstream>
#include <GLFW/glfw3native.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>
#include "../Game/Camera.h"
#include "../Helpers/EntityHelpers.h"
#include "../Helpers/MapCheck.h"
#include "../Logging/Logger.h"
#include "../Game/Time.h"

#include "../Game/Entity/Entity.h"
#include "../Game/Components/StaticModelComponent.h"
#include "../Game/Components/TransformComponent.h"

#include "PC_VertexTypes.h"

using namespace glm;

bgfx::VertexLayout StaticVertex::layout;
bgfx::VertexLayout WorldVertex::layout;

// static WorldVertex worldPlane[] = {
//     {vec3(-1.0f, 0.0f,-1.0f) * 4.0f, vec3(0.0f, 1.0f, 0.0f)},
//     {vec3( 1.0f, 0.0f,-1.0f) * 4.0f, vec3(0.0f, 1.0f, 0.0f)},
//     {vec3( 1.0f, 0.0f, 1.0f) * 4.0f, vec3(0.0f, 1.0f, 0.0f)},
//     {vec3(-1.0f, 0.0f, 1.0f) * 4.0f, vec3(0.0f, 1.0f, 0.0f)},
// };
//
// static uint16_t worldVertices[] = {
//     0, 2, 1,
//     0, 3, 2,
// };

const uint16_t PLANE_SIZE = 64;
static WorldVertex worldPlane[PLANE_SIZE * PLANE_SIZE ];
static uint16_t worldIndices[(PLANE_SIZE - 1) * (PLANE_SIZE -1) * 6];

const uint16_t NOISE_SIZE = 4096;
static float textureData[NOISE_SIZE][NOISE_SIZE];

Renderer::Renderer(GLFWwindow* window) {
    DEBUGLOG("Starting BGFX...");
    bgfx::Init init; 
    init.type = bgfx::RendererType::Count;
    init.resolution.width = 1280;
    init.resolution.height = 720;
    init.resolution.reset = BGFX_RESET_VSYNC;
    init.platformData.nwh = GETHANDLE(window);
    bgfx::init(init);
    DEBUGLOG("Succsefully started BGFX");

    width_ = 1280;
    height_ = 720;
    projectionMatrix_ = perspectiveFovRH_ZO(radians(70.0f), (float)width_, (float)height_, 0.5f, 1000.0f);

    for (int i = 0; i < MAX_TEXTURES_PER_MATERIAL; i++) {
        std::string samplerName = "s_sampler" + std::to_string(i);
        samplers_[i] = bgfx::createUniform(samplerName.c_str(), bgfx::UniformType::Sampler);
    }

    u_normal_ = bgfx::createUniform("u_normal", bgfx::UniformType::Mat3);
    u_timeResolution_ = bgfx::createUniform("u_timeResolution", bgfx::UniformType::Vec4);
    u_cameraPosition_ = bgfx::createUniform("u_cameraPosition", bgfx::UniformType::Vec4);
    u_cameraUp_ = bgfx::createUniform("u_cameraUp", bgfx::UniformType::Vec4);
    u_cameraRight_ = bgfx::createUniform("u_cameraRight", bgfx::UniformType::Vec4);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, 1280, 720);

    StaticVertex::Init();
    WorldVertex::Init();

    for (int x = 0; x < PLANE_SIZE; x++)
    for (int y = 0; y < PLANE_SIZE; y++) {
        uint16_t index = y * PLANE_SIZE + x;
        worldPlane[index] = {vec3(x, 0.0f, y), vec3(0.0f, 0.0f, 0.0f)};
    };

    uint32_t count = 0;
    for (int x = 0; x < PLANE_SIZE - 1; x++)
    for (int y = 0; y < PLANE_SIZE - 1; y++) {
        uint16_t i0 = (y + 0) * PLANE_SIZE + (x + 0);
        uint16_t i1 = (y + 1) * PLANE_SIZE + (x + 0);
        uint16_t i2 = (y + 1) * PLANE_SIZE + (x + 1);
        uint16_t i3 = (y + 0) * PLANE_SIZE + (x + 1);
        
        worldIndices[count++] = i0;
        worldIndices[count++] = i1;
        worldIndices[count++] = i2;

        worldIndices[count++] = i2;
        worldIndices[count++] = i3;
        worldIndices[count++] = i0;
    };

    for (int x = 0; x < NOISE_SIZE; x++) 
    for (int y = 0; y < NOISE_SIZE; y++)  {
        float x2 = (NOISE_SIZE / 2.0f - x);
        x2 *= x2;
        float y2 = (NOISE_SIZE / 2.0f - y);
        y2 *= y2;
        textureData[x][y] = glm::sqrt(x2 + y2) / NOISE_SIZE;
    };

    noiseTexture_ = bgfx::createTexture2D(NOISE_SIZE, NOISE_SIZE, false, 1, 
        bgfx::TextureFormat::R32F, 
        BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE,
        bgfx::makeRef(textureData, sizeof(textureData))
    );

    worldMesh_.vertexBuffer = bgfx::createVertexBuffer(bgfx::makeRef(worldPlane, sizeof(worldPlane)), WorldVertex::layout);
    worldMesh_.indexBuffer = bgfx::createIndexBuffer(bgfx::makeRef(worldIndices, sizeof(worldIndices)));

    TEMP_LoadTestData();
}

void Renderer::TEMP_LoadTestData() {
    LoadVertexShader_P("StaticVS");
    LoadFragmentShader_P("DefaultFS");
    LoadModel_P("st_sphere");
    LoadTexture_P("bricks_c");
    LoadTexture_P("bricks_n");

    std::string textures[] = {"bricks_c", "bricks_n"};
    MakeMaterial_P("playerMaterial", "StaticVS", "DefaultFS", textures, 2);
    DEBUGLOG("Succesfully loaded all test assets");

    LoadVertexShader_P("WorldVS");
    LoadFragmentShader_P("WorldFS");
    worldMaterial_.textures[0] = noiseTexture_;
    worldMaterial_.numTextures = 1;
    worldMaterial_.shader = bgfx::createProgram(GetVertexShader("WorldVS"), GetFragmentShader("WorldFS"));

    DEBUGLOG("Create world material");
}

void Renderer::StartFrame_P() {
    viewMatrix_ = camera_->GetViewMatrix();
    bgfx::setViewTransform(0, &viewMatrix_,&projectionMatrix_);

    vec4 timeResolution = vec4(Time::GetTime(), 1280, 720, 0.0f);
    vec4 cameraPosition = vec4(camera_->transform_.position_, 0.0f);
    vec4 cameraUp = vec4(camera_->transform_.GetUpVector(), 0.0f);
    vec4 cameraRight = vec4(camera_->transform_.GetRightVector(), 0.0f);

    bgfx::setUniform(u_timeResolution_, &timeResolution);
    bgfx::setUniform(u_cameraPosition_, &cameraPosition);
    bgfx::setUniform(u_cameraUp_, &cameraUp);
    bgfx::setUniform(u_cameraRight_, &cameraRight);

    bgfx::touch(0);
}

void Renderer::Clear_P() {
    return;
}

void Renderer::PresentFrame_P() {
    bgfx::frame(); 
}

void Renderer::RenderWorld_P(World& world) {
    Transform emptyTransform;
    mat4 worldMatrix = emptyTransform.GetWorldMatrix();
    bgfx::setTransform(&worldMatrix);

    for (int i = 0; i < worldMaterial_.numTextures; i++)
        bgfx::setTexture(i, samplers_[i], worldMaterial_.textures[i]);

    bgfx::setVertexBuffer(0, worldMesh_.vertexBuffer);
    bgfx::setIndexBuffer(worldMesh_.indexBuffer);
    bgfx::submit(0, worldMaterial_.shader);
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
        bgfx::setUniform(u_normal_, &normalMatrix);

        Model model = staticModelComponent.model[i];
        for (int m = 0; m < model.numMeshes; m++) {
            Material material = staticModelComponent.materials[i][m];
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

#define MEMORYFROMFILE(path)                                    \
    const bgfx::Memory* memory = nullptr;                       \
    std::ifstream file;                                         \
    file.open(path, std::ios::binary);                          \
    if (!file.is_open())                                        \
        ERRORLOG("BGFX failed to open file: " << path);         \
                                                                \
    file.seekg(0, file.end);                                    \
    size_t fileSize = file.tellg();                             \
    file.seekg(0, file.beg);                                    \
                                                                \
    memory = bgfx::alloc(fileSize);                             \
    file.read((char*)memory->data, fileSize);                   \
    file.close()


void Renderer::LoadVertexShader_P(std::string name) {
    ForceMapUnique(vertexShaders_, name, "Vertex shader " + name + " is already loaded");
    std::string path = "./shaders/" + name + ".bin";
    MEMORYFROMFILE(path);
    if (memory == nullptr)
        abort();

    vertexShaders_[name] = bgfx::createShader(memory);
    DEBUGLOG("Loaded vertex shader " << name);
}

void Renderer::LoadFragmentShader_P(std::string name) {
    ForceMapUnique(fragmentShaders_, name, "Fragment shader " + name + " is already loaded");
    std::string path = "./shaders/" + name + ".bin";
    MEMORYFROMFILE(path);
    if (memory == nullptr)
        abort();

    fragmentShaders_[name] = bgfx::createShader(memory);
    DEBUGLOG("Loaded fragment shader " << name);
}

void Renderer::LoadModel_P(std::string name) {
    ForceMapUnique(models_, name, "Model " + name + " is already loaded");
    Model model;
    std::ifstream file;
    file.open("./models/" + name + ".jmd", std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        ERRORLOG("Failed to load model " << name);
        abort();
    }

    ModelFileHeader modelHeader;
    file.read((char*)&modelHeader, sizeof(ModelFileHeader));
    model.numMeshes = modelHeader.numMeshes;

    for (int i = 0; i < model.numMeshes; i++) {
        MeshFileHeader meshHeader;
        file.read((char*)&meshHeader, sizeof(MeshFileHeader));
        
        const bgfx::Memory* vertexMem = bgfx::alloc(sizeof(StaticVertex) * meshHeader.numVertices);
        file.read((char*)vertexMem->data, sizeof(StaticVertex) * meshHeader.numVertices);

        const bgfx::Memory* indexMem = bgfx::alloc(sizeof(uint16_t) * meshHeader.numIndices);
        file.read((char*)indexMem->data, sizeof(uint16_t) * meshHeader.numIndices);

        model.meshes[i].vertexBuffer = bgfx::createVertexBuffer(vertexMem, StaticVertex::layout);
        model.meshes[i].indexBuffer = bgfx::createIndexBuffer(indexMem);
    }
    models_[name] = model;

    DEBUGLOG("Loaded model " << name << " with " << (int)model.numMeshes << " meshes");
}

void Renderer::LoadTexture_P(std::string name) {
    ForceMapUnique(textures_, name, "Texture " + name + " is already loaded");
    std::string path = "./textures/" + name + ".dds";
    MEMORYFROMFILE(path);
    if (memory == nullptr)
        abort();

    textures_[name] = bgfx::createTexture(memory);
    DEBUGLOG("Loaded texture " << name);
}

void Renderer::MakeMaterial_P(
    std::string name, 
    std::string vertex, 
    std::string fragment, 
    std::string textures[MAX_TEXTURES_PER_MATERIAL], 
    uint8_t numTextures
) {
    ForceMapUnique(materials_, name, "Material " + name + " is already loaded");
    Material material;
    Shader vertexShader = GetVertexShader(vertex);
    Shader fragmentShader = GetFragmentShader(fragment);
    material.shader = bgfx::createProgram(vertexShader, fragmentShader);
    material.numTextures = numTextures;
    for (int i = 0; i < numTextures; i++)
        material.textures[i] = GetTexture(textures[i]);
    materials_[name] = material;

    DEBUGLOG(
        "Created material " << name << '\n' << 
        "\tVertex Shader: " << vertex << '\n' <<
        "\tFragment Shader: " << fragment << '\n' <<
        "\tNumber of textures: " << (int)numTextures
    );
}
