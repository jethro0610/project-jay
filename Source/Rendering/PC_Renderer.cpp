#include "Renderer.h"
#include <fstream>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <FastNoiseLite.h>
#include <random>
#include <json.hpp>
#include "../Game/Camera.h"
#include "../Game/Entity/EntityKey.h"
#include "../Helpers/MapCheck.h"
#include "../Helpers/LoadHelpers.h"
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

const float SHADOW_DISTANCE = 1000.0f;
const float SHADOW_FORWARD = 40.0f;
const float SHADOW_RANGE = 80.0f;
const int SHADOW_RESOLUTION = 2048;

const int SHADOW_VIEW = 0;
const int RENDER_VIEW = 1;
const int POST_PROCESS_VIEW = 2;
const int UI_VIEW = 3;

Renderer::Renderer(FastNoiseLite& noise, GLFWwindow* window) {
    renderWidth_ = 1920;
    renderHeight_ = 1080;
    width_ = 1280;
    height_ = 720;

    DEBUGLOG("Starting BGFX...");
    bgfx::Init init; 
    init.type = bgfx::RendererType::Count;
    init.resolution.width = width_;
    init.resolution.height = height_;
    init.resolution.reset = BGFX_RESET_NONE;
    init.platformData.nwh = GETHANDLE(window);
    init.platformData.ndt = GETDISPLAY();
    bgfx::init(init);
    DEBUGLOG("Succesfully started BGFX");

    projectionMatrix_ = perspectiveFovRH_ZO(radians(70.0f), (float)width_, (float)height_, 0.5f, 1000.0f);
    shadowProjectionMatrix_ = orthoRH_ZO(-SHADOW_RANGE, SHADOW_RANGE, -SHADOW_RANGE, SHADOW_RANGE, 0.5f, SHADOW_DISTANCE);

    for (int i = 0; i < NUM_SAMPLERS; i++) {
        std::string samplerName = "s_sampler" + std::to_string(i);
        samplers_[i] = bgfx::createUniform(samplerName.c_str(), bgfx::UniformType::Sampler);
    }
    u_shadowMatrix_ = bgfx::createUniform("u_shadowMatrix", bgfx::UniformType::Mat4);
    u_shadowResolution_ = bgfx::createUniform("u_shadowResolution", bgfx::UniformType::Vec4);
    u_shadowUp_ = bgfx::createUniform("u_shadowUp", bgfx::UniformType::Vec4);
    u_shadowRight_ = bgfx::createUniform("u_shadowRight", bgfx::UniformType::Vec4);

    u_materialProps_ = bgfx::createUniform("u_materialProps", bgfx::UniformType::Mat4);
    u_normalMult_ = bgfx::createUniform("u_normalMult", bgfx::UniformType::Vec4);
    u_lightDirection_ = bgfx::createUniform("u_lightDirection", bgfx::UniformType::Vec4);
    u_timeResolution_ = bgfx::createUniform("u_timeResolution", bgfx::UniformType::Vec4);
    u_cameraPosition_ = bgfx::createUniform("u_cameraPosition", bgfx::UniformType::Vec4);
    u_cameraUp_ = bgfx::createUniform("u_cameraUp", bgfx::UniformType::Vec4);
    u_cameraRight_ = bgfx::createUniform("u_cameraRight", bgfx::UniformType::Vec4);
    u_randomVec_ = bgfx::createUniform("u_randomVec", bgfx::UniformType::Vec4);
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
    InitShadowBuffer_P();
    InitRenderBuffer_P();
    InitPostProcessBuffer_P();
    InitUIBuffer_P();

    TEMP_LoadTestData();
}

void Renderer::TEMP_LoadTestData() {
    Model sphere = LoadModel_P("st_sphere");
    Model rock = LoadModel_P("st_rock");
    Model tree = LoadModel_P("st_tree");
    Model flower = LoadModel_P("st_flower");
    Model character = LoadModel_P("st_char");
    spreadModel_ = flower;

    Texture bricksC = LoadTexture_P("t_bricks_c");
    Texture bricksN = LoadTexture_P("t_bricks_n");
    Texture grassC = LoadTexture_P("t_grass_c");
    Texture grassN = LoadTexture_P("t_grass_n");
    Texture marbleC = LoadTexture_P("t_marble_c");
    Texture rockC = LoadTexture_P("t_rock_c");
    Texture rockN = LoadTexture_P("t_rock_n");
    Texture crackM = LoadTexture_P("t_crack_m");
    Texture treeC = LoadTexture_P("t_tree_c");
    Texture treeN = LoadTexture_P("t_tree_n");
    Texture leavesM = LoadTexture_P("t_leaves_m");
    Texture flowerM = LoadTexture_P("t_flower_m");
    Texture font = LoadTexture_P("t_font");
    Texture hairM = LoadTexture_P("t_hair_m");

    LoadVertexShader_P("vs_static");
    LoadVertexShader_P("vs_static_s");
    LoadVertexShader_P("vs_static_crack");
    LoadVertexShader_P("vs_inst");
    LoadVertexShader_P("vs_inst_s");
    LoadVertexShader_P("vs_inst_billboard");
    LoadVertexShader_P("vs_inst_billboard_s");
    LoadVertexShader_P("vs_world");
    LoadVertexShader_P("vs_world_s");
    LoadVertexShader_P("vs_screenquad");
    LoadVertexShader_P("vs_glyph");
    LoadVertexShader_P("vs_uibar");

    LoadFragmentShader_P("fs_depth_s");
    LoadFragmentShader_P("fs_depth_masked_s");

    LoadFragmentShader_P("fs_dfsa");
    LoadFragmentShader_P("fs_dfsa_color");
    LoadFragmentShader_P("fs_dfsa_color_masked");
    LoadFragmentShader_P("fs_dfsa_crack");

    LoadFragmentShader_P("fs_flower");
    LoadFragmentShader_P("fs_seed");
    LoadFragmentShader_P("fs_leaves_strand");
    LoadFragmentShader_P("fs_world");

    LoadFragmentShader_P("fs_blit");
    LoadFragmentShader_P("fs_text");
    LoadFragmentShader_P("fs_uibar");
    LoadFragmentShader_P("fs_postprocess");

    LoadMaterial_P("m_player");
    LoadMaterial_P("m_playerskin");
    LoadMaterial_P("m_playershirt");
    LoadMaterial_P("m_hair");
    LoadMaterial_P("m_rock");
    LoadMaterial_P("m_tree");
    LoadMaterial_P("m_willowleaves");
    worldMaterial_ = LoadMaterial_P("m_world");
    postProcessMaterial_ = LoadMaterial_P("m_postprocess");
    spreadMaterials_[0] = LoadMaterial_P("m_flower");
    spreadMaterials_[1] = LoadMaterial_P("m_stem");
    seedMaterial_ = LoadMaterial_P("m_seed");
    textMaterial_ = LoadMaterial_P("m_text");
    barMaterial_ = LoadMaterial_P("m_uibar");
    blitMaterial_ = LoadMaterial_P("m_preuiblit");

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

// TODO: InitBuffer functions into one generic function
void Renderer::InitShadowBuffer_P() {
    shadowBufferTexture_ = bgfx::createTexture2D(
        SHADOW_RESOLUTION,
        SHADOW_RESOLUTION,
        false,
        1,
        bgfx::TextureFormat::D16,
        BGFX_TEXTURE_RT | BGFX_SAMPLER_UVW_BORDER
    );
    shadowBuffer_ = bgfx::createFrameBuffer(1, &shadowBufferTexture_);

    bgfx::setViewFrameBuffer(SHADOW_VIEW, shadowBuffer_);
    bgfx::setViewClear(SHADOW_VIEW, BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);
    bgfx::setViewRect(SHADOW_VIEW, 0, 0, SHADOW_RESOLUTION, SHADOW_RESOLUTION);

    vec4 shadowResolution = vec4(SHADOW_RESOLUTION, SHADOW_RESOLUTION, 0.0f, 0.0f);
    bgfx::setUniform(u_shadowResolution_, &shadowResolution);
    textures_["t_g_shadowMap"] = shadowBufferTexture_;
}

void Renderer::InitRenderBuffer_P() {
    renderBufferTextures_[0] = bgfx::createTexture2D(
        renderWidth_,
        renderHeight_,
        false,
        1,
        bgfx::TextureFormat::BGRA8,
        BGFX_TEXTURE_RT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP
    );
    renderBufferTextures_[1] = bgfx::createTexture2D(
        renderWidth_,
        renderHeight_,
        false,
        1,
        bgfx::TextureFormat::D16,
        BGFX_TEXTURE_RT | BGFX_TEXTURE_RT_WRITE_ONLY
    );
    renderBuffer_ = bgfx::createFrameBuffer(2, renderBufferTextures_);

    bgfx::setViewFrameBuffer(RENDER_VIEW, renderBuffer_);
    bgfx::setViewClear(RENDER_VIEW, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);
    bgfx::setViewRect(RENDER_VIEW, 0, 0, renderWidth_, renderHeight_);
    textures_["t_g_render_c"] = renderBufferTextures_[0];
    textures_["t_g_render_d"] = renderBufferTextures_[1];
}

void Renderer::InitPostProcessBuffer_P() {
    postProcessTexture_ = bgfx::createTexture2D(
        renderWidth_,
        renderHeight_,
        false,
        1,
        bgfx::TextureFormat::BGRA8,
        BGFX_TEXTURE_RT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP
    );

    postProcessBuffer_ = bgfx::createFrameBuffer(1, &postProcessTexture_);
    bgfx::setViewFrameBuffer(POST_PROCESS_VIEW, postProcessBuffer_);
    bgfx::setViewClear(POST_PROCESS_VIEW, BGFX_CLEAR_COLOR, 0x000000FF, 1.0f, 0);
    bgfx::setViewRect(POST_PROCESS_VIEW, 0, 0, renderWidth_, renderHeight_);
    textures_["t_g_post_c"] = postProcessTexture_;
}

void Renderer::InitUIBuffer_P() {
    bgfx::setViewFrameBuffer(UI_VIEW, backBuffer_);
    bgfx::setViewClear(UI_VIEW, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);
    bgfx::setViewRect(UI_VIEW, 0, 0, width_, height_);
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
    bgfx::setViewTransform(RENDER_VIEW, &viewMatrix_, &projectionMatrix_);

    vec3 forward = camera_->transform_.GetForwardVector();
    forward.y = 0.0f;
    forward = normalize(forward);

    vec3 cameraPos = camera_->transform_.position;
    cameraPos.y = 0.0f;

    vec3 lookPosition = cameraPos + forward * SHADOW_FORWARD;
    vec3 lightPosition = -lightDirection_ * SHADOW_DISTANCE * 0.75f + lookPosition;
    shadowViewMatrix_ = lookAtRH(lightPosition, lookPosition, Transform::worldUp);
    shadowMatrix_ = shadowProjectionMatrix_ * shadowViewMatrix_;
    bgfx::setViewTransform(SHADOW_VIEW, &shadowViewMatrix_, &shadowProjectionMatrix_);

    vec4 timeResolution = vec4(GlobalTime::GetTime(), width_, height_, 0.0f);
    vec4 cameraPosition = vec4(camera_->transform_.position, 0.0f);
    vec4 cameraUp = vec4(camera_->transform_.GetUpVector(), 0.0f);
    vec4 cameraRight = vec4(camera_->transform_.GetRightVector(), 0.0f);
    vec4 randomVec = vec4(rand());

    bgfx::setUniform(u_timeResolution_, &timeResolution);
    bgfx::setUniform(u_cameraPosition_, &cameraPosition);
    bgfx::setUniform(u_cameraUp_, &cameraUp);
    bgfx::setUniform(u_cameraRight_, &cameraRight);
    bgfx::setUniform(u_randomVec_, &randomVec);
    bgfx::setUniform(u_shadowMatrix_, &shadowMatrix_);

    bgfx::touch(0);
}

void Renderer::Clear_P() {
    return;
}

void Renderer::PresentFrame_P() {
    bgfx::frame(); 
}

void Renderer::RenderMesh_P(Mesh* mesh, Material* material, InstanceBuffer* instanceBuffer, glm::mat4* worldMatrix) {
    vec4 normalMult;

    int numOfRenders = 1;
    if (material->castShadows)
        numOfRenders *= 2;
    if (material->triangleType > ONE_SIDED)
        numOfRenders *= 2;

    int curPass = 0;
    int curFace = 0;

    mat4 transposeProps = transpose(material->properties);
    bgfx::setUniform(u_materialProps_, &transposeProps);
    for (int n = 0; n < numOfRenders; n++) {
        if (worldMatrix != nullptr)
            bgfx::setTransform(worldMatrix);

        if (curFace == 1) {
            bgfx::setState(BGFX_STATE_DEFAULT | BGFX_STATE_FRONT_CCW);
            normalMult.x = material->triangleType == TWO_SIDED_NEGATIVE_BACK ? -1.0f : 1.0f;
            normalMult.y = -1.0f;
        }
        else {
            normalMult.x = 1.0f;
            normalMult.y = 1.0f;
        }
        bgfx::setUniform(u_normalMult_, &normalMult);

        int view;
        MaterialShader shader;
        if (curPass == 0) {
            view = RENDER_VIEW;
            shader = material->shader;
            SetTexturesFromMaterial_P(*material, true);
        }
        else {
            view = SHADOW_VIEW;
            shader = material->shadowShader;
            SetTexturesFromMaterial_P(*material, false);
        }

        if (instanceBuffer != nullptr)
            bgfx::setInstanceDataBuffer(instanceBuffer);

        bgfx::setVertexBuffer(0, mesh->vertexBuffer);
        bgfx::setIndexBuffer(mesh->indexBuffer);
        bgfx::submit(view, shader);

        if (material->triangleType > ONE_SIDED) {
            curFace++;

            // When two faces have been rendered,
            // render to the next pass
            if (curFace > 1) {
                curFace = 0;
                curPass = 1; 
            }
        }
        else
            curPass++;
    }
}

void Renderer::RenderWorld_P(World& world) {
    vec4 worldProps[2];
    worldProps[0].x = 0.0f;
    worldProps[0].y = world.properties_.minRadius;
    worldProps[0].z = world.properties_.maxRadius;
    worldProps[0].w = world.properties_.edgeJaggedness;
    worldProps[1].x = world.properties_.edgeFalloff;
    worldProps[1].y = world.properties_.edgePower;

    // Can use instancing here if necessary
    int radius = world.properties_.maxRadius / WORLD_MESH_SIZE;
    radius += 1;
    for (int x = -radius; x < radius; x++)
    for (int y = -radius; y < radius; y++) { 
        bgfx::setUniform(u_worldProps_, worldProps, 2);
        vec4 offset = vec4(x * WORLD_MESH_SIZE, 0.0f, y * WORLD_MESH_SIZE, 0.0f);
        bgfx::setUniform(u_worldMeshOffset_, &offset);

        bgfx::setTexture(WORLD_NOISE_TEXINDEX, samplers_[WORLD_NOISE_TEXINDEX], noiseTexture_);
        RenderMesh_P(&worldMesh_, &worldMaterial_);
    };
}

EntityKey constexpr key = GetEntityKey<StaticModelComponent, TransformComponent>();
void Renderer::RenderEntities_P(
    Entity* entities, 
    MeterComponent& meterComponent,
    StaticModelComponent& staticModelComponent,
    TransformComponent& transformComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;
        if (!entity.MatchesKey(key))
            continue;

        vec4 meter = vec4(meterComponent.meter[i], meterComponent.maxMeter[i], 0.0f, 0.0f); 
        bgfx::setUniform(u_meter_, &meter);
        mat4 worldMatrix = transformComponent.renderTransform[i].GetWorldMatrix();

        Model model = staticModelComponent.model[i];
        for (int m = 0; m < model.numMeshes; m++) {
            Material material = staticModelComponent.materials[i][m];
            Mesh mesh = model.meshes[m];
            RenderMesh_P(&mesh, &material, nullptr, &worldMatrix);
        }
    }
}

void Renderer::RenderSpread_P(SpreadManager& spreadManager) {
    uint32_t count = spreadManager.GetCount();
    if (count == 0)
        return;

    bgfx::InstanceDataBuffer instanceBuffer;
    bgfx::allocInstanceDataBuffer(&instanceBuffer, count, sizeof(SpreadRenderData));
    memcpy(instanceBuffer.data, spreadManager.GetRenderData(), sizeof(SpreadRenderData) * count);

    for (int m = 0; m < spreadModel_.numMeshes; m++) {
        Mesh mesh = spreadModel_.meshes[m];
        Material material = spreadMaterials_[m];
        RenderMesh_P(&mesh, &material, &instanceBuffer);
    }
}

void Renderer::RenderSeed_P(SeedManager& seedManager) {
    uint32_t count = seedManager.seeds_.GetCount();
    if (count == 0)
        return;
    
    bgfx::InstanceDataBuffer instanceBuffer;
    bgfx::allocInstanceDataBuffer(&instanceBuffer, count, sizeof(vec4));
    memcpy(instanceBuffer.data, seedManager.positions_, sizeof(vec4) * count);

    RenderMesh_P(&quad_, &seedMaterial_, &instanceBuffer);
}

void Renderer::RenderPostProcess_P() {
    for (int i = 0; i < postProcessMaterial_.numTextures; i++)
        bgfx::setTexture(i, samplers_[i], postProcessMaterial_.textures[i]);

    bgfx::setVertexBuffer(0, quad_.vertexBuffer);
    bgfx::setIndexBuffer(quad_.indexBuffer);
    bgfx::submit(POST_PROCESS_VIEW, postProcessMaterial_.shader);
}

void Renderer::RenderBlit_P() {
    bgfx::setTexture(0, samplers_[0], blitMaterial_.textures[0]);
    bgfx::setVertexBuffer(0, quad_.vertexBuffer);
    bgfx::setIndexBuffer(quad_.indexBuffer);
    bgfx::submit(UI_VIEW, blitMaterial_.shader);
}

void Renderer::RenderUI_P(MeterComponent& meterComponent) {
    vec4 meter = vec4(meterComponent.meter[PLAYER_ENTITY], meterComponent.maxMeter[PLAYER_ENTITY], 0.0f, 0.0f); 
    bgfx::setUniform(u_meter_, &meter);

    bgfx::setVertexBuffer(0, quad_.vertexBuffer);
    bgfx::setIndexBuffer(quad_.indexBuffer);
    bgfx::submit(UI_VIEW, barMaterial_.shader);
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
    bgfx::submit(UI_VIEW, textMaterial_.shader);
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
    model.meshes.resize(modelHeader.numMeshes);

    for (int i = 0; i < model.meshes.size(); i++) {
        MeshFileHeader meshHeader;
        file.read((char*)&meshHeader, sizeof(MeshFileHeader));
        
        const bgfx::Memory* vertexMem = bgfx::alloc(sizeof(StaticVertex) * meshHeader.numVertices);
        file.read((char*)vertexMem->data, sizeof(StaticVertex) * meshHeader.numVertices);

        const bgfx::Memory* indexMem = bgfx::alloc(sizeof(uint16_t) * meshHeader.numIndices);
        file.read((char*)indexMem->data, sizeof(uint16_t) * meshHeader.numIndices);

        model.meshes[i].vertexBuffer = bgfx::createVertexBuffer(vertexMem, StaticVertex::layout);
        model.meshes[i].indexBuffer = bgfx::createIndexBuffer(indexMem);
    }

    // Skeleton_INTERNAL skeleton;
    // skeleton.joints.resize(modelHeader.numJoints);
    // file.read((char*)skeleton.joints.data(), sizeof(Joint) * skeleton.joints.size());
    // skeletons_[name] = skeleton;

    DEBUGLOG("Loaded model " << name << " with " << (int)model.meshes.size() << " meshes");
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

Material Renderer::LoadMaterial_P(std::string name) {
    std::ifstream inFile("materials/" + name + ".json");
    ASSERT(inFile.is_open(), "Failed to load material " + name);
    nlohmann::json data = nlohmann::json::parse(inFile);
    Material material;

    material.triangleType = ONE_SIDED;
    if (GetBoolean(data, "two_sided"))
        material.triangleType = TWO_SIDED;
    if (GetBoolean(data, "negative_back") && material.triangleType == TWO_SIDED)
        material.triangleType = TWO_SIDED_NEGATIVE_BACK;

    Shader vertexShader = GetVertexShader(GetString(data, "vertex"));
    Shader fragmentShader = GetFragmentShader(GetString(data, "fragment"));
    material.shader = bgfx::createProgram(vertexShader, fragmentShader);

    material.castShadows = GetBoolean(data, "cast_shadows");
    if (material.castShadows) {
        Shader vertexShadowShader = GetVertexShader(GetString(data, "vertex_shadow"));
        Shader fragmentShadowShader = GetFragmentShader(GetString(data, "fragment_shadow"));
        material.shadowShader = bgfx::createProgram(vertexShadowShader, fragmentShadowShader);
    }

    if (data.contains("textures")) {
        auto textureNames = data["textures"];
        for (int i = 0; i < textureNames.size(); i++)
            material.textures[i] = GetTexture(textureNames[i]);
        material.numTextures = textureNames.size();
    }

    material.properties[0][0] = GetFloat(data, "specular_power", 32.0f);
    material.properties[0][1] = GetFloat(data, "specular_threshhold", 0.3f);
    material.properties[0][2] = GetFloat(data, "specular_brightness", 1.5f);

    material.properties[1][0] = GetFloat(data, "fresnel_power", 4.0f);
    material.properties[1][1] = GetFloat(data, "fresnel_scale", 1.0f);
    material.properties[1][2] = GetFloat(data, "fresnel_brightness", 1.0f);

    material.properties[3] = GetVec4(data, "color");
    
    materials_[name] = material;
    DEBUGLOG("Loaded material " << name);
    return material;
}

void Renderer::SetTexturesFromMaterial_P(Material& material, bool shadowMap) {
    for (int i = 0; i < material.numTextures; i++)
        bgfx::setTexture(i, samplers_[i], material.textures[i]);

    if (shadowMap)
        bgfx::setTexture(SHADOW_TEXINDEX, samplers_[SHADOW_TEXINDEX], shadowBufferTexture_);
}

void Renderer::SetLightDirection_P(vec3 direction) {
    quat directionQuat = quatLookAtRH(normalize(direction), Transform::worldUp);
    lightDirection_ = rotate(directionQuat, Transform::worldForward);
    vec4 shadowRight = vec4(rotate(directionQuat, Transform::worldRight), 0.0f);
    vec4 shadowUp = vec4(rotate(directionQuat, Transform::worldUp), 0.0f);

    vec4 uniformDirection = normalize(vec4(lightDirection_, 0.0f));
    bgfx::setUniform(u_lightDirection_, &uniformDirection);
    bgfx::setUniform(u_shadowRight_, &shadowRight);
    bgfx::setUniform(u_shadowUp_, &shadowUp);
}
