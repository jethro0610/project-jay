#include "ResourceManager.h"
#include "Rendering/PC_VertexTypes.h"
#include "Rendering/ShadowConstants.h"
#include "Terrain/Terrain.h"
#include "Helpers/Assert.h"
#include "Helpers/LoadHelpers.h"
#include "VertexShaders.h"
#include "FragmentShaders.h"

using namespace glm;

bgfx::VertexLayout StaticVertex::layout;
bgfx::VertexLayout SkeletalVertex::layout;
bgfx::VertexLayout WorldVertex::layout;
bgfx::VertexLayout TextureQuadVertex::layout;

ResourceManager::ResourceManager() {
    StaticVertex::Init();
    SkeletalVertex::Init();
    WorldVertex::Init();
    TextureQuadVertex::Init();
    CreateTerrainMapTexture();
    LoadGlobals();
    LoadAllShaders();
}

void ResourceManager::CreateTerrainMapTexture() {
    if (!textures_.Has("t_terrainmap")) {
        textures_.Add("t_terrainmap") = { bgfx::createTexture2D(
            TerrainConsts::RESOLUTION, 
            TerrainConsts::RESOLUTION, 
            false, 
            1, 
            bgfx::TextureFormat::RG32F,
            BGFX_TEXTURE_NONE | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP,
            NULL
        )};
    }

    ASSIGN_DEBUG_NAME(textures_.Get("t_terrainmap"), "t_terrainmap");
    globals_.insert("t_terrainmap");
}

void ResourceManager::UpdateTerrainMapTexture(glm::vec2* terrainMap) {
    bgfx::updateTexture2D(
        textures_.Get("t_terrainmap").handle,
        0,
        0,
        0,
        0,
        TerrainConsts::RESOLUTION,
        TerrainConsts::RESOLUTION,
        bgfx::copy(terrainMap, sizeof(glm::vec2) * TerrainConsts::RESOLUTION * TerrainConsts::RESOLUTION)
    );
}

void ResourceManager::LoadGlobals() {
    LoadRenderTextures();
    LoadGlobalQuad();
    LoadGlobalTerrain();

    
    LoadModel("st_default");
    LoadTexture("t_font");
    globals_.insert("st_default");
    globals_.insert("t_font");

    // TEST GLOBALS
    LoadModel("st_flower_test");
    LoadTexture("t_grass_c");
    LoadTexture("t_grass_n");
    LoadTexture("t_marble_c");
    LoadTexture("t_flower_m");
    globals_.insert("st_flower_test");
    globals_.insert("t_grass_c");
    globals_.insert("t_grass_n");
    globals_.insert("t_marble_c");
    globals_.insert("t_flower_m");
}

void ResourceManager::LoadRenderTextures() {
    textures_.Add("t_shadowmap") = { bgfx::createTexture2D(
        ShadowConstants::SHADOW_RESOLUTION,
        ShadowConstants::SHADOW_RESOLUTION,
        false,
        1,
        bgfx::TextureFormat::D16,
        BGFX_TEXTURE_RT | BGFX_SAMPLER_UVW_BORDER
    )};
    ASSIGN_DEBUG_NAME(textures_.Get("t_shadowmap"), "t_shadowmap");
    globals_.insert("t_shadowmap");

    textures_.Add("t_render_c") = { bgfx::createTexture2D(
        1920,
        1080,
        false,
        1,
        bgfx::TextureFormat::BGRA8,
        BGFX_TEXTURE_RT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP
    )};
    globals_.insert("t_render_c");

    textures_.Add("t_render_d") = { bgfx::createTexture2D(
        1920,
        1080,
        false,
        1,
        bgfx::TextureFormat::D16,
        BGFX_TEXTURE_RT | BGFX_TEXTURE_RT_WRITE_ONLY
    )};
    ASSIGN_DEBUG_NAME(textures_.Get("t_render_d"), "t_render_d");
    globals_.insert("t_render_d");

    textures_.Add("t_post_c") = { bgfx::createTexture2D(
        1920,
        1080,
        false,
        1,
        bgfx::TextureFormat::BGRA8,
        BGFX_TEXTURE_RT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP
    )};
    ASSIGN_DEBUG_NAME(textures_.Get("t_post_c"), "t_post_c");
    globals_.insert("t_post_c");
}

void ResourceManager::LoadGlobalQuad() {
    Model& quad = models_.Add("st_quad");
    ASSIGN_DEBUG_NAME(quad, "st_quad");
    Mesh& quadMesh = quad.meshes.push_back();

    TextureQuadVertex vertices[4];
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
    globals_.insert("st_quad");
}

void ResourceManager::LoadGlobalTerrain() {
    Model& terrainModel = models_.Add("st_terrainsheet");
    ASSIGN_DEBUG_NAME(terrainModel, "st_terrainsheet");
    Mesh& terrainMesh = terrainModel.meshes.push_back();

    WorldVertex vertices[TerrainConsts::NUM_VERTICES];
    for (int x = 0; x < TerrainConsts::SIZE; x++)
    for (int y = 0; y < TerrainConsts::SIZE; y++) {
        uint16_t index = y * TerrainConsts::SIZE + x;
        vec3 position = vec3(x / TerrainConsts::MESH_DENSITY, 0.0f, y / TerrainConsts::MESH_DENSITY);
        vertices[index] = { position };
    };
    terrainMesh.vertexBuffer = bgfx::createVertexBuffer(
        bgfx::copy(
            vertices, 
            sizeof(WorldVertex) * TerrainConsts::NUM_VERTICES 
        ), 
        WorldVertex::layout
    );
    DEBUGLOG("Created terrain mesh with " << TerrainConsts::NUM_VERTICES << " vertices");
    
    uint16_t terrainIndices[TerrainConsts::NUM_INDICES];
    int count = 0;
    for (int x = 0; x < TerrainConsts::SIZE - 1; x++)
    for (int y = 0; y < TerrainConsts::SIZE - 1; y++) {
        uint16_t i0 = (y + 0) * TerrainConsts::SIZE + (x + 0);
        uint16_t i1 = (y + 1) * TerrainConsts::SIZE + (x + 0);
        uint16_t i2 = (y + 1) * TerrainConsts::SIZE + (x + 1);
        uint16_t i3 = (y + 0) * TerrainConsts::SIZE + (x + 1);
        
        terrainIndices[count++] = i0;
        terrainIndices[count++] = i1;
        terrainIndices[count++] = i2;

        terrainIndices[count++] = i2;
        terrainIndices[count++] = i3;
        terrainIndices[count++] = i0;
    };
    terrainMesh.indexBuffer = bgfx::createIndexBuffer(
        bgfx::copy(
            terrainIndices, 
            sizeof(uint16_t) * TerrainConsts::NUM_INDICES
        )
    );
    globals_.insert("st_terrainsheet");
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

void ResourceManager::LoadAllShaders() {
    bgfx::ShaderHandle vertexShaders[NUM_VERTEX_SHADERS];
    for (int i = 0; i < NUM_VERTEX_SHADERS; i++) {
        std::string path = "./shaders/" + std::string(VERTEX_SHADERS[i]) + ".bin";

        MEMORYFROMFILE(path);
        if (memory == nullptr)
            abort(); 
        vertexShaders[i] = bgfx::createShader(memory);
    }

    bgfx::ShaderHandle fragmentShaders[NUM_FRAGMENT_SHADERS];
    for (int i = 0; i < NUM_FRAGMENT_SHADERS; i++) {
        std::string path = "./shaders/" + std::string(FRAGMENT_SHADERS[i]) + ".bin";

        MEMORYFROMFILE(path);
        if (memory == nullptr)
            abort(); 
        fragmentShaders[i] = bgfx::createShader(memory);
    }

    for (int v = 0; v < NUM_VERTEX_SHADERS; v++) { 
    for (int f = 0; f < NUM_FRAGMENT_SHADERS; f++) { 
        bgfx::ProgramHandle handle = bgfx::createProgram(vertexShaders[v], fragmentShaders[f]);
        if (!bgfx::isValid(handle))
            continue;

        std::string name = std::string(VERTEX_SHADERS[v]) + "." + std::string(FRAGMENT_SHADERS[f]);
        Shader& shader = shaders_.Add(name);
        shader.DBG_name = name;
        shader.handle = handle;
    } }
}

void ResourceManager::LoadTexture(const std::string& name) {
    Texture& texture = textures_.Add(name);
    ASSIGN_DEBUG_NAME(texture, name);
    std::string path = "./textures/" + name + ".dds";
    MEMORYFROMFILE(path);
    if (memory == nullptr)
        abort();

    texture.handle = bgfx::createTexture(memory);
    DEBUGLOG("Loaded texture " << name);
}
void ResourceManager::UnloadTexture(const std::string& name) {
    bgfx::destroy(textures_.Get(name).handle);
    DEBUGLOG("Unloaded texture " << name);
    textures_.Remove(name);
}

void ResourceManager::LoadModel(const std::string& name) {
    Model& model = models_.Add(name);
    ASSIGN_DEBUG_NAME(model, name);
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
    bgfx::VertexLayout& layout = modelHeader.skeletal ? SkeletalVertex::layout : StaticVertex::layout;
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

    Skeleton& skeleton = skeletons_.Add(name);
    file.read((char*)&skeleton.bones_, sizeof(Bones));
    file.read((char*)&skeleton.ribbons_, sizeof(Ribbons));
    DEBUGLOG("Loaded skeleton " << name);

    assert(modelHeader.numAnimations <= Animation::MAX_ANIMATIONS);
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
    DEBUGLOG(
        "Loaded skeletal model " << name << " with " << 
        (int)model.meshes.size() << " meshes" << " and " <<
        modelHeader.numAnimations << " animations"
    );
}
void ResourceManager::UnloadModel(const std::string& name) {
    Model& model = models_.Get(name);
    for (Mesh& mesh : model.meshes) {
        bgfx::destroy(mesh.indexBuffer);
        bgfx::destroy(mesh.vertexBuffer);
    }
    
    DEBUGLOG("Unloaded model " << name);
    models_.Remove(name);

    if (skeletons_.Has(name))
        skeletons_.Remove(name);
}
