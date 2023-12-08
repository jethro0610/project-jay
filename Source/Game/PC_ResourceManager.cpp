#include "ResourceManager.h"
#include "../Helpers/MapCheck.h"
#include "../Helpers/Assert.h"
#include "../Helpers/LoadHelpers.h"

#include "../Rendering/PC_VertexTypes.h"
using namespace glm;

bgfx::VertexLayout StaticVertex::layout;
bgfx::VertexLayout SkeletalVertex::layout;
bgfx::VertexLayout WorldVertex::layout;
bgfx::VertexLayout TextureQuadVertex::layout;

const float WORLD_MESH_SIZE = 64.0f;
const float WORLD_MESH_DENSITY = 0.5f;

const float SHADOW_DISTANCE = 1000.0f;
const float SHADOW_FORWARD = 60.0f;
const float SHADOW_RANGE = 120.0f;
const int SHADOW_RESOLUTION = 2048;

ResourceManager::ResourceManager() {
    StaticVertex::Init();
    SkeletalVertex::Init();
    WorldVertex::Init();
    TextureQuadVertex::Init();
    LoadGlobals();
}

// TODO: Noise texture

void ResourceManager::LoadGlobals() {
    textures_["t_g_noise"] = bgfx::createTexture2D(NOISE_RESOLUTION, NOISE_RESOLUTION, false, 1, 
        bgfx::TextureFormat::R32F,
        BGFX_TEXTURE_NONE | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP,
        bgfx::copy(noise_.GetData(), sizeof(float) * NOISE_RESOLUTION * NOISE_RESOLUTION)
    );
    LoadRenderTextures();
    LoadGlobalQuad();
    LoadGlobalTerrain();
}

void ResourceManager::LoadRenderTextures() {
    textures_["t_g_shadow"] = bgfx::createTexture2D(
        SHADOW_RESOLUTION,
        SHADOW_RESOLUTION,
        false,
        1,
        bgfx::TextureFormat::D16,
        BGFX_TEXTURE_RT | BGFX_SAMPLER_UVW_BORDER
    );
    globals_.insert("t_g_shadow");

    textures_["t_g_render_c"] = bgfx::createTexture2D(
        1920,
        1080,
        false,
        1,
        bgfx::TextureFormat::BGRA8,
        BGFX_TEXTURE_RT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP
    );
    globals_.insert("t_g_render_c");

    textures_["t_g_render_d"] = bgfx::createTexture2D(
        1920,
        1080,
        false,
        1,
        bgfx::TextureFormat::D16,
        BGFX_TEXTURE_RT | BGFX_TEXTURE_RT_WRITE_ONLY
    );
    globals_.insert("t_g_render_d");

    textures_["t_g_post_c"] = bgfx::createTexture2D(
        1920,
        1080,
        false,
        1,
        bgfx::TextureFormat::BGRA8,
        BGFX_TEXTURE_RT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP
    );
    globals_.insert("t_g_post_c");
}

void ResourceManager::LoadGlobalQuad() {
    TextureQuadVertex vertices[4];
    Model quad;
    Mesh quadMesh;
    vertices[0] = { glm::vec2(-0.5f, -0.5f), glm::vec2( 0.0f, 1.0f) };
    vertices[1] = { glm::vec2( 0.5f, -0.5f), glm::vec2( 1.0f, 1.0f) };
    vertices[2] = { glm::vec2( 0.5f,  0.5f), glm::vec2( 1.0f, 0.0f) };
    vertices[3] = { glm::vec2(-0.5f,  0.5f), glm::vec2( 0.0f, 0.0f) };
    quadMesh.vertexBuffer = bgfx::createVertexBuffer(bgfx::copy(vertices, sizeof(vertices)), TextureQuadVertex::layout);
    
    uint16_t indices[6];
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 2;
    indices[4] = 3;
    indices[5] = 0;
    quadMesh.indexBuffer = bgfx::createIndexBuffer(bgfx::copy(indices, sizeof(indices)));
    quad.meshes.push_back(quadMesh);
    models_["st_g_quad"] = quad;
    globals_.insert("st_g_quad");
}

void ResourceManager::LoadGlobalTerrain() {
    Mesh worldMesh;
    int size = ceil(WORLD_MESH_SIZE * WORLD_MESH_DENSITY) + 1;

    int numVertices = size * size;
    WorldVertex* vertices =  new WorldVertex[numVertices];
    for (int x = 0; x < size; x++)
    for (int y = 0; y < size; y++) {
        uint16_t index = y * size + x;
        vec3 position = vec3(x / WORLD_MESH_DENSITY, 0.0f, y / WORLD_MESH_DENSITY);
        vertices[index] = { position };
    };
    worldMesh.vertexBuffer = bgfx::createVertexBuffer(
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
    worldMesh.indexBuffer = bgfx::createIndexBuffer(
        bgfx::copy(
            worldIndices, 
            sizeof(uint16_t) * numIndices
        )
    );
    delete[] worldIndices;

    Model worldModel;
    worldModel.meshes.push_back(worldMesh);
    models_["st_g_terrain"] = worldModel;
}

#define MEMORYFROMFILE(path)                                    \
    const bgfx::Memory* memory = nullptr;                       \
    std::ifstream file;                                         \
    file.open(path, std::ios::binary);                          \
    if (!file.is_open()) {                                      \
        ERRORLOG("BGFX failed to open file: " << path);         \
        abort();                                                \
    }                                                           \
                                                                \
    file.seekg(0, file.end);                                    \
    size_t fileSize = file.tellg();                             \
    file.seekg(0, file.beg);                                    \
                                                                \
    memory = bgfx::alloc(fileSize);                             \
    file.read((char*)memory->data, fileSize);                   \
    file.close()

void ResourceManager::LoadVertexShader(const std::string& name) {
    ForceMapUnique(vertexShaders_, name, "Vertex shader " + name + " is already loaded");
    std::string path = "./shaders/" + name + ".bin";
    MEMORYFROMFILE(path);
    if (memory == nullptr)
        abort();

    ShaderHandle shader = bgfx::createShader(memory);
    DEBUGLOG("Loaded vertex shader " << name);
    vertexShaders_[name] = shader;
}

void ResourceManager::LoadFragmentShader(const std::string& name) {
    ForceMapUnique(fragmentShaders_, name, "Fragment shader " + name + " is already loaded");
    std::string path = "./shaders/" + name + ".bin";
    MEMORYFROMFILE(path);
    if (memory == nullptr)
        abort();

    ShaderHandle shader = bgfx::createShader(memory);
    DEBUGLOG("Loaded fragment shader " << name);
    fragmentShaders_[name] = shader;
}

void ResourceManager::LoadTexture(const std::string& name) {
    ForceMapUnique(textures_, name, "Texture " + name + " is already loaded");
    std::string path = "./textures/" + name + ".dds";
    MEMORYFROMFILE(path);
    if (memory == nullptr)
        abort();

    TextureHandle texture = bgfx::createTexture(memory);
    DEBUGLOG("Loaded texture " << name);
    textures_[name] = texture;
}

void ResourceManager::LoadMaterial(const std::string& name) {
    std::ifstream inFile("materials/" + name + ".json");
    ASSERT(inFile.is_open(), "Failed to load material " + name);
    nlohmann::json data = nlohmann::json::parse(inFile);
    Material material;

    material.triangleType = ONE_SIDED;
    if (GetBoolean(data, "two_sided"))
        material.triangleType = TWO_SIDED;
    if (GetBoolean(data, "negative_back") && material.triangleType == TWO_SIDED)
        material.triangleType = TWO_SIDED_NEGATIVE_BACK;

    ShaderHandle vertexShader = GetVertexShader(GetString(data, "vertex"));
    ShaderHandle fragmentShader = GetFragmentShader(GetString(data, "fragment"));
    material.shader = bgfx::createProgram(vertexShader, fragmentShader);

    material.castShadows = GetBoolean(data, "cast_shadows");
    if (material.castShadows) {
        ShaderHandle vertexShadowShader = GetVertexShader(GetString(data, "vertex_shadow"));
        ShaderHandle fragmentShadowShader = GetFragmentShader(GetString(data, "fragment_shadow"));
        material.shadowShader = bgfx::createProgram(vertexShadowShader, fragmentShadowShader);
    }

    if (data.contains("textures")) {
        auto& textureNames = data["textures"];
        ASSERT((textureNames.size() <= MAX_TEXTURES_PER_MATERIAL), "Too many textures on material " + name);
        for (const std::string& textureName : textureNames)
            material.textures.push_back(GetTexture(textureName));
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
}

void ResourceManager::LoadModel(const std::string& name) {
    ForceMapUnique(models_, name, "Model " + name + " is already loaded");
    Model& model = models_[name];
    std::ifstream file;
    file.open("./models/" + name + ".jmd", std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        ERRORLOG("Failed to load model " << name);
        abort();
    }

    ModelFileHeader modelHeader;
    file.read((char*)&modelHeader, sizeof(ModelFileHeader));
    model.meshes.resize(modelHeader.numMeshes);

    int vertexSize = modelHeader.skeletal ? sizeof(SkeletalVertex) : sizeof(StaticVertex);
    bgfx::VertexLayout layout = modelHeader.skeletal ? SkeletalVertex::layout : StaticVertex::layout;
    for (Mesh& mesh : model.meshes) {
        MeshFileHeader meshHeader;
        file.read((char*)&meshHeader, sizeof(MeshFileHeader));
        
        const bgfx::Memory* vertexMem = bgfx::alloc(vertexSize * meshHeader.numVertices);
        file.read((char*)vertexMem->data, vertexSize * meshHeader.numVertices);

        const bgfx::Memory* indexMem = bgfx::alloc(sizeof(uint16_t) * meshHeader.numIndices);
        file.read((char*)indexMem->data, sizeof(uint16_t) * meshHeader.numIndices);

        mesh.vertexBuffer = bgfx::createVertexBuffer(vertexMem, layout);
        mesh.indexBuffer = bgfx::createIndexBuffer(indexMem);
    }

    if (!modelHeader.skeletal) {
        file.close();
        DEBUGLOG("Loaded static model " << name << " with " << (int)model.meshes.size() << " meshes");
        return;
    }

    Skeleton& skeleton = skeletons_[name];
    file.read((char*)&skeleton.bones_, sizeof(Bones));
    file.read((char*)&skeleton.ribbons_, sizeof(Ribbons));
    DEBUGLOG("Loaded skeleton " << name);

    assert(modelHeader.numAnimations <= MAX_ANIMATIONS);
    skeleton.animations_.resize(modelHeader.numAnimations);
    for (int i = 0; i < modelHeader.numAnimations; i++) {
        AnimationHeader animationHeader;
        file.read((char*)&animationHeader, sizeof(AnimationHeader));

        Animation& animation = skeleton.animations_[i];
        animation.keyframes.resize(animationHeader.numKeyframes);
        animation.framerate = animationHeader.framerate;
        animation.speedInfluence = animationHeader.speedInfluence;
        file.read((char*)animation.keyframes.data(), sizeof(Keyframe) * animationHeader.numKeyframes);
        DEBUGLOG("Loaded animation " << animationHeader.name << " with " << animation.keyframes.size() << " keyframes");
    }

    file.close();
    models_[name] = model;
    DEBUGLOG(
        "Loaded skeletal model " << name << " with " << 
        (int)model.meshes.size() << " meshes" << " and " <<
        modelHeader.numAnimations << " animations"
    );
}

void ResourceManager::LoadEntityDescription(const std::string& name) {
    ForceMapUnique(entityDescs_, name, "Entity description " + name + " is already loaded");

    std::ifstream inFile("entities/" + name + ".json");
    ASSERT(inFile.is_open(), "Failed to load entity " + name);

    entityDescs_[name] = nlohmann::json::parse(inFile);
    DEBUGLOG("Loaded entity " << name);
}

void ResourceManager::LoadEmitterProperties(const std::string& name) {
    ForceMapUnique(emitterProps_, name, "Emitter " + name + " is already loaded");

    std::ifstream inFile("emitters/" + name + ".json");
    ASSERT(inFile.is_open(), "Failed to load emitter" + name);

    nlohmann::json data = nlohmann::json::parse(inFile);
    EmitterProperties properties;

    properties.material = GetMaterial(GetString(data, "material", "null_material"));
    properties.localSpace = GetBoolean(data, "local_space");

    properties.spawnInterval = GetFloat(data, "spawn_interval", 1.0f);
    properties.spawnCount = GetInt(data, "spawn_count", 1);
    properties.lifetime = GetFloat(data, "lifetime", 1.0f);

    auto& scaleRange = data["scale_range"];
    properties.minScale = scaleRange[0];
    properties.maxScale = scaleRange[1];
    properties.endScale = GetFloat(data, "end_scale");

    properties.spawnRadius = GetFloat(data, "spawn_radius", 0.0f);
    auto& velocityRange = data["velocity_range"];
    properties.minVelocity = GetVec4(velocityRange[0]);
    properties.maxVelocity = GetVec4(velocityRange[1]);
    properties.acceleration = GetVec4(data, "acceleration");

    properties.startColor = GetVec4(data, "start_color");
    properties.endColor = GetVec4(data, "end_color");

    emitterProps_[name] = properties; 
    DEBUGLOG("Loaded emitter " << name);
}
