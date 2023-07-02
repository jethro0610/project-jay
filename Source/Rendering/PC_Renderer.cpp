#include "Renderer.h"
#include <fstream>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <FastNoiseLite.h>
#include "../Game/Camera.h"
#include "../Game/Entity/EntityKey.h"
#include "../Helpers/MapCheck.h"
#include "../Logging/Logger.h"
#include "../Game/Time.h"

#include "../Game/Entity/Entity.h"
#include "../Game/Components/StaticModelComponent.h"
#include "../Game/Components/TransformComponent.h"

#include "../Game/World/SpreadManager.h"
#include "../Game/World/SeedManager.h"

#include "../Game/Components/MeterComponent.h"

#include "PC_VertexTypes.h"

using namespace glm;

bgfx::VertexLayout StaticVertex::layout;
bgfx::VertexLayout WorldVertex::layout;
bgfx::VertexLayout TextureQuadVertex::layout;

const float WORLD_MESH_SIZE = 64.0f;
const float WORLD_MESH_DENSITY = 0.5f;

Renderer::Renderer(FastNoiseLite& noise, GLFWwindow* window) {
    DEBUGLOG("Starting BGFX again...");
    bgfx::Init init; 
    init.type = bgfx::RendererType::Count;
    init.resolution.width = 1280;
    init.resolution.height = 720;
    init.resolution.reset = BGFX_RESET_NONE;
    init.platformData.nwh = GETHANDLE(window);
    init.platformData.ndt = GETDISPLAY();
    bgfx::init(init);
    DEBUGLOG("Succesfully started BGFX");

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
    u_lightDirection_ = bgfx::createUniform("u_lightDirection", bgfx::UniformType::Vec4);
    u_meter_ = bgfx::createUniform("u_meter", bgfx::UniformType::Vec4);
    u_worldProps_ = bgfx::createUniform("u_worldProps", bgfx::UniformType::Vec4, 2);
    u_worldMeshOffset_= bgfx::createUniform("u_worldMeshOffset", bgfx::UniformType::Vec4);
    u_noiseProps_ = bgfx::createUniform("u_noiseProps", bgfx::UniformType::Vec4);

    SetLightDirection_P(vec3(1.0f, -1.0f, 1.0f));

    StaticVertex::Init();
    WorldVertex::Init();
    TextureQuadVertex::Init();

    noiseTexture_ = MakeNoiseTexture_P(noise, 4096, 1024);
    vec4 noiseProps;
    noiseProps.x = 1024;
    noiseProps.y = 1.0f / (1024 * 2.0f);
    bgfx::setUniform(u_noiseProps_, &noiseProps);


    backBuffer_ = BGFX_INVALID_HANDLE;
    InitWorldMesh_P();
    InitQuad_P();
    InitRenderBuffer_P();
    InitPostProcessBuffer_P();
    InitUIBuffer_P();

    TEMP_LoadTestData();
}

void Renderer::TEMP_LoadTestData() {
    Model sphere = LoadModel_P("st_sphere");
    Texture bricksC = LoadTexture_P("bricks_c");
    Texture bricksN = LoadTexture_P("bricks_n");
    Texture grassC = LoadTexture_P("grass_c");
    Texture grassN = LoadTexture_P("grass_n");
    Texture marbleC = LoadTexture_P("marble_c");

    Shader staticVS = LoadVertexShader_P("StaticVS");
    Shader defaultFS = LoadFragmentShader_P("DefaultFS");
    Texture playerTextures[] = { bricksC, bricksN };
    Material playerMaterial = MakeMaterial_P("player", staticVS, defaultFS, playerTextures, 2);

    Shader worldVS = LoadVertexShader_P("WorldVS");
    Shader worldFS = LoadFragmentShader_P("WorldFS");
    Texture worldTextures[] = { noiseTexture_, grassC, grassN, marbleC };
    worldMaterial_ = MakeMaterial_P("world", worldVS, worldFS, worldTextures, 4);

    Shader screenQuadVS = LoadVertexShader_P("ScreenQuadVS");
    Shader postProcessFS = LoadFragmentShader_P("PostProcessFS");
    Texture postProcessTextures[] = { renderBufferTextures_[0] };
    postProcessMaterial_ = MakeMaterial_P("postProcess", screenQuadVS, postProcessFS, postProcessTextures, 1);

    Shader instanceVS = LoadVertexShader_P("InstanceVS");
    spreadMaterial_ = MakeMaterial_P("spread", instanceVS, defaultFS, playerTextures, 2); 
    spreadModel_ = sphere;

    Shader instBillboardVS = LoadVertexShader_P("InstBillboardVS");
    Shader seedFS = LoadFragmentShader_P("SeedFS");
    seedMaterial_ = MakeMaterial_P("seed", instBillboardVS, seedFS, nullptr, 0);

    Shader glyphVS = LoadVertexShader_P("GlyphVS");
    Shader textFS = LoadFragmentShader_P("TextFS");
    Texture fontTextures[] = { LoadTexture_P("font") };
    textMaterial_ = MakeMaterial_P("text", glyphVS, textFS, fontTextures, 1);

    Shader barVS = LoadVertexShader_P("BarVS");
    Shader barFS = LoadFragmentShader_P("BarFS");
    barMaterial_ = MakeMaterial_P("bar", barVS, barFS, nullptr, 0);

    Shader blitFS = LoadFragmentShader_P("BlitFS");
    Texture blitTextures[] = { postProcessTexture_ };
    blitMaterial_ = MakeMaterial_P("blit", screenQuadVS, blitFS, blitTextures, 1);

    DEBUGLOG("Succesfully loaded all test assets");
}

void Renderer::InitQuad_P() {
    TextureQuadVertex vertices[4];
    vertices[0] = { glm::vec2(-0.5f, -0.5f), glm::vec2( 0.0f, 1.0f) };
    vertices[1] = { glm::vec2( 0.5f, -0.5f), glm::vec2( 1.0f, 1.0f) };
    vertices[2] = { glm::vec2( 0.5f,  0.5f), glm::vec2( 1.0f, 0.0f) };
    vertices[3] = { glm::vec2(-0.5f,  0.5f), glm::vec2( 0.0f, 0.0f) };
    quad_.vertexBuffer = bgfx::createVertexBuffer(bgfx::copy(vertices, sizeof(vertices)), TextureQuadVertex::layout);
    
    uint16_t indices[6];
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 2;
    indices[4] = 3;
    indices[5] = 0;
    quad_.indexBuffer = bgfx::createIndexBuffer(bgfx::copy(indices, sizeof(indices)));
}

void Renderer::InitRenderBuffer_P() {
    renderBufferTextures_[0] = bgfx::createTexture2D(
        width_,
        height_,
        false,
        1,
        bgfx::TextureFormat::BGRA8,
        BGFX_TEXTURE_RT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP
    );
    renderBufferTextures_[1] = bgfx::createTexture2D(
        width_,
        height_,
        false,
        1,
        bgfx::TextureFormat::D16,
        BGFX_TEXTURE_RT | BGFX_TEXTURE_RT_WRITE_ONLY
    );
    renderBuffer_ = bgfx::createFrameBuffer(2, renderBufferTextures_);

    bgfx::setViewFrameBuffer(0, renderBuffer_);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, 1280, 720);
}

void Renderer::InitPostProcessBuffer_P() {
    postProcessTexture_ = bgfx::createTexture2D(
        width_,
        height_,
        false,
        1,
        bgfx::TextureFormat::BGRA8,
        BGFX_TEXTURE_RT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP
    );

    postProcessBuffer_ = bgfx::createFrameBuffer(1, &postProcessTexture_);
    bgfx::setViewFrameBuffer(1, postProcessBuffer_);
    bgfx::setViewClear(1, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);
    bgfx::setViewRect(1, 0, 0, 1280, 720);
}

void Renderer::InitUIBuffer_P() {
    bgfx::setViewFrameBuffer(2, backBuffer_);
    bgfx::setViewClear(2, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);
    bgfx::setViewRect(2, 0, 0, 1280, 720);
}

void Renderer::InitWorldMesh_P() {
    int size = ceil(WORLD_MESH_SIZE * WORLD_MESH_DENSITY) + 1;

    int numVertices = size * size;
    WorldVertex* vertices =  new WorldVertex[numVertices];
    for (int x = 0; x < size; x++)
    for (int y = 0; y < size; y++) {
        uint16_t index = y * size + x;
        vec3 position = vec3(x / WORLD_MESH_DENSITY, 0.0f, y / WORLD_MESH_DENSITY);
        vertices[index] = { position };
    };
    worldMesh_.vertexBuffer = bgfx::createVertexBuffer(
        bgfx::copy(
            vertices, 
            sizeof(WorldVertex) * numVertices
        ), 
        WorldVertex::layout
    );
    DEBUGLOG("Created world mesh with " << numVertices << " vertices");
    delete[] vertices;
    
    int numIndices = (size - 1) * (size - 1) * 6;
    uint16_t* worldIndices = new uint16_t[numIndices];
    int count = 0;
    for (int x = 0; x < size - 1; x++)
    for (int y = 0; y < size - 1; y++) {
        uint16_t i0 = (y + 0) * size + (x + 0);
        uint16_t i1 = (y + 1) * size + (x + 0);
        uint16_t i2 = (y + 1) * size + (x + 1);
        uint16_t i3 = (y + 0) * size + (x + 1);
        
        worldIndices[count++] = i0;
        worldIndices[count++] = i1;
        worldIndices[count++] = i2;

        worldIndices[count++] = i2;
        worldIndices[count++] = i3;
        worldIndices[count++] = i0;
    };
    worldMesh_.indexBuffer = bgfx::createIndexBuffer(
        bgfx::copy(
            worldIndices, 
            sizeof(uint16_t) * numIndices
        )
    );
    delete[] worldIndices;
}

Texture Renderer::MakeNoiseTexture_P(FastNoiseLite& noise, int resolution, float distance) {
    Texture texture;
    float halfResolution = resolution /  2.0f;
    float scale = distance / halfResolution;

    float* textureData = new float[resolution * resolution];
    for (int x = 0; x < resolution; x++) 
    for (int y = 0; y < resolution; y++)  {
        float offsetX = (x - halfResolution) * scale;
        float offsetY = (y - halfResolution) * scale;
        int index = y * resolution + x;

        textureData[index] = noise.GetNoise(offsetX, offsetY);
    };

    texture = bgfx::createTexture2D(resolution, resolution, false, 1, 
        bgfx::TextureFormat::R32F,
        BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE,
        bgfx::copy(textureData, sizeof(float) * resolution * resolution)
    );
    delete[] textureData;

    return texture;
}

void Renderer::StartFrame_P() {
    viewMatrix_ = camera_->GetViewMatrix();
    bgfx::setViewTransform(0, &viewMatrix_,&projectionMatrix_);

    vec4 timeResolution = vec4(GlobalTime::GetTime(), 1280, 720, 0.0f);
    vec4 cameraPosition = vec4(camera_->transform_.position, 0.0f);
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
    vec4 worldProps[2];
    worldProps[0].x = 0.0f;
    worldProps[0].y = world.minRadius_;
    worldProps[0].z = world.maxRadius_;
    worldProps[0].w = world.edgeJaggedness_;
    worldProps[1].x = world.edgeFalloff_;
    worldProps[1].y = world.edgePower_;

    // Can use instancing here if necessary
    int radius = world.maxRadius_ / WORLD_MESH_SIZE;
    for (int x = -radius; x < radius; x++)
    for (int y = -radius; y < radius; y++) { 
        bgfx::setUniform(u_worldProps_, worldProps, 2);
        vec4 offset = vec4(x * WORLD_MESH_SIZE, 0.0f, y * WORLD_MESH_SIZE, 0.0f);
        bgfx::setUniform(u_worldMeshOffset_, &offset);

        for (int i = 0; i < worldMaterial_.numTextures; i++)
            bgfx::setTexture(i, samplers_[i], worldMaterial_.textures[i]);

        bgfx::setVertexBuffer(0, worldMesh_.vertexBuffer);
        bgfx::setIndexBuffer(worldMesh_.indexBuffer);
        bgfx::submit(0, worldMaterial_.shader);
    };
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
    uint32_t count = spreadManager.GetCount();
    if (count == 0)
        return;

    bgfx::InstanceDataBuffer instanceBuffer;
    bgfx::allocInstanceDataBuffer(&instanceBuffer, count, sizeof(vec4));
    memcpy(instanceBuffer.data, spreadManager.GetPositions(), sizeof(vec4) * count);
    bgfx::setInstanceDataBuffer(&instanceBuffer);

    for (int m = 0; m < spreadModel_.numMeshes; m++) {
        Mesh mesh = spreadModel_.meshes[m];

        for (int t = 0; t < spreadMaterial_.numTextures; t++)
            bgfx::setTexture(t, samplers_[t], spreadMaterial_.textures[t]);

        bgfx::setVertexBuffer(0, mesh.vertexBuffer);
        bgfx::setIndexBuffer(mesh.indexBuffer);
        bgfx::submit(0, spreadMaterial_.shader);
    }
}

void Renderer::RenderSeed_P(SeedManager& seedManager) {
    uint32_t count = seedManager.seeds_.GetCount();
    if (count == 0)
        return;
    
    bgfx::InstanceDataBuffer instanceBuffer;
    bgfx::allocInstanceDataBuffer(&instanceBuffer, count, sizeof(vec4));
    memcpy(instanceBuffer.data, seedManager.positions_, sizeof(vec4) * count);
    bgfx::setInstanceDataBuffer(&instanceBuffer);

    bgfx::setVertexBuffer(0, quad_.vertexBuffer);
    bgfx::setIndexBuffer(quad_.indexBuffer);
    bgfx::submit(0, seedMaterial_.shader);
}

void Renderer::RenderPostProcess_P() {
    for (int i = 0; i < postProcessMaterial_.numTextures; i++)
        bgfx::setTexture(i, samplers_[i], postProcessMaterial_.textures[i]);

    bgfx::setVertexBuffer(0, quad_.vertexBuffer);
    bgfx::setIndexBuffer(quad_.indexBuffer);
    bgfx::submit(1, postProcessMaterial_.shader);
}

void Renderer::RenderBlit_P() {
    bgfx::setTexture(0, samplers_[0], blitMaterial_.textures[0]);
    bgfx::setVertexBuffer(0, quad_.vertexBuffer);
    bgfx::setIndexBuffer(quad_.indexBuffer);
    bgfx::submit(2, blitMaterial_.shader);
}

void Renderer::RenderUI_P(MeterComponent& meterComponent) {
    vec4 meter = vec4((float)meterComponent.meter[PLAYER_ENTITY] / meterComponent.maxMeter[PLAYER_ENTITY]); 
    bgfx::setUniform(u_meter_, &meter);

    bgfx::setVertexBuffer(0, quad_.vertexBuffer);
    bgfx::setIndexBuffer(quad_.indexBuffer);
    bgfx::submit(2, barMaterial_.shader);
}

void Renderer::RenderScreenText_P() {
    if (!ScreenText::IsEnabled())
        return;

    bgfx::setState(BGFX_STATE_CULL_CW | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_ALPHA);
    uint32_t count = MAX_LINES * CHARS_PER_LINE;
    
    bgfx::InstanceDataBuffer instanceBuffer;
    bgfx::allocInstanceDataBuffer(&instanceBuffer, count, sizeof(vec4));
    memcpy(instanceBuffer.data, ScreenText::GetText(), sizeof(vec4) * count);
    bgfx::setInstanceDataBuffer(&instanceBuffer);

    bgfx::setTexture(0, samplers_[0], textMaterial_.textures[0]);

    bgfx::setVertexBuffer(0, quad_.vertexBuffer);
    bgfx::setIndexBuffer(quad_.indexBuffer);
    bgfx::submit(2, textMaterial_.shader);
    bgfx::setState(BGFX_STATE_DEFAULT);
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


Shader Renderer::LoadVertexShader_P(std::string name) {
    ForceMapUnique(vertexShaders_, name, "Vertex shader " + name + " is already loaded");
    std::string path = "./shaders/" + name + ".bin";
    MEMORYFROMFILE(path);
    if (memory == nullptr)
        abort();

    Shader shader = bgfx::createShader(memory);
    DEBUGLOG("Loaded vertex shader " << name);
    vertexShaders_[name] = shader;
    return shader;
}

Shader Renderer::LoadFragmentShader_P(std::string name) {
    ForceMapUnique(fragmentShaders_, name, "Fragment shader " + name + " is already loaded");
    std::string path = "./shaders/" + name + ".bin";
    MEMORYFROMFILE(path);
    if (memory == nullptr)
        abort();

    Shader shader = bgfx::createShader(memory);
    DEBUGLOG("Loaded fragment shader " << name);
    fragmentShaders_[name] = shader;
    return shader;
}

Model Renderer::LoadModel_P(std::string name) {
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

    DEBUGLOG("Loaded model " << name << " with " << (int)model.numMeshes << " meshes");
    models_[name] = model;
    return model;
}

Texture Renderer::LoadTexture_P(std::string name) {
    ForceMapUnique(textures_, name, "Texture " + name + " is already loaded");
    std::string path = "./textures/" + name + ".dds";
    MEMORYFROMFILE(path);
    if (memory == nullptr)
        abort();

    Texture texture = bgfx::createTexture(memory);
    DEBUGLOG("Loaded texture " << name);
    textures_[name] = texture;
    return texture;
}

Material Renderer::MakeMaterial_P(
    std::string name, 
    Shader vertex, 
    Shader fragment, 
    Texture textures[MAX_TEXTURES_PER_MATERIAL], 
    int numTextures
) {
    ForceMapUnique(materials_, name, "Material " + name + " is already loaded");
    Material material;
    material.shader = bgfx::createProgram(vertex, fragment);
    material.numTextures = numTextures;
    for (int i = 0; i < numTextures; i++)
        material.textures[i] = textures[i];
    materials_[name] = material;

    DEBUGLOG("Created material " << name);
    return material;
}

Material Renderer::MakeMaterial_P(
    std::string name, 
    std::string vertex, 
    std::string fragment, 
    std::string textures[MAX_TEXTURES_PER_MATERIAL], 
    int numTextures
) {
    Texture textureList[MAX_TEXTURES_PER_MATERIAL];
    for (int i = 0; i < numTextures; i++)
        textureList[i] = GetTexture(textures[i]);

    return MakeMaterial_P(name, GetVertexShader(vertex), GetFragmentShader(fragment), textureList, numTextures);
}

void Renderer::SetLightDirection_P(vec3 direction) {
    vec4 lightDirection = normalize(vec4(direction, 0.0f));
    bgfx::setUniform(u_lightDirection_, &lightDirection);
}
