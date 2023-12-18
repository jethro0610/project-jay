#include "ResourceManager.h"
#include "Rendering/PC_VertexTypes.h"
#include "Rendering/ShadowConstants.h"
#include "Terrain/Terrain.h"
#include "Helpers/Assert.h"
#include "Helpers/LoadHelpers.h"

using namespace glm;

bgfx::VertexLayout StaticVertex::layout;
bgfx::VertexLayout SkeletalVertex::layout;
bgfx::VertexLayout WorldVertex::layout;
bgfx::VertexLayout TextureQuadVertex::layout;

ResourceManager::ResourceManager(Terrain& terrain) {
    StaticVertex::Init();
    SkeletalVertex::Init();
    WorldVertex::Init();
    TextureQuadVertex::Init();
    GenerateHeightmapTexture(terrain);
    LoadGlobals();
}

void ResourceManager::GenerateHeightmapTexture(Terrain& terrain) {
    terrain.GenerateHeightmap();
    textures_.Add("t_heightmap") = { bgfx::createTexture2D(
        Terrain::RESOLUTION, 
        Terrain::RESOLUTION, 
        false, 
        1, 
        bgfx::TextureFormat::R32F,
        BGFX_TEXTURE_NONE | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP,
        bgfx::copy(terrain.GetHeightmap(), sizeof(float) * Terrain::RESOLUTION * Terrain::RESOLUTION)
    )};
    ASSIGN_DEBUG_NAME(textures_.Get("t_heightmap"), "t_heightmap");
    globals_.insert("t_heightmap");
}

void ResourceManager::LoadGlobals() {
    LoadRenderTextures();
    LoadGlobalQuad();
    LoadGlobalTerrain();
    LoadGlobalFile();
}

void ResourceManager::LoadGlobalFile() {
    std::ifstream inFile("levels/load_globals.json");
    ASSERT(inFile.is_open(), "Failed to load global file");

    auto globals = nlohmann::json::parse(inFile);
    for (auto& vertexShader : globals["vertex_shaders"]) {
        LoadVertexShader(vertexShader);
        globals_.insert(vertexShader);
    }
    for (auto& fragmentShader : globals["fragment_shaders"]) {
        LoadFragmentShader(fragmentShader);
        globals_.insert(fragmentShader);
    }
    for (auto& texture: globals["textures"]) {
        LoadTexture(texture);
        globals_.insert(texture);
    }
    for (auto& material : globals["materials"]) {
        LoadMaterial(material);
        globals_.insert(material);
    }
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
    int size = ceil(Terrain::TERRAIN_MESH_SIZE * Terrain::TERRAIN_MESH_DENSITY) + 1;

    int numVertices = size * size;
    WorldVertex* vertices =  new WorldVertex[numVertices];
    for (int x = 0; x < size; x++)
    for (int y = 0; y < size; y++) {
        uint16_t index = y * size + x;
        vec3 position = vec3(x / Terrain::TERRAIN_MESH_DENSITY, 0.0f, y / Terrain::TERRAIN_MESH_DENSITY);
        vertices[index] = { position };
    };
    terrainMesh.vertexBuffer = bgfx::createVertexBuffer(
        bgfx::copy(
            vertices, 
            sizeof(WorldVertex) * numVertices
        ), 
        WorldVertex::layout
    );
    DEBUGLOG("Created terrain mesh with " << numVertices << " vertices");
    delete[] vertices;
    
    int numIndices = (size - 1) * (size - 1) * 6;
    uint16_t* terrainIndices = new uint16_t[numIndices];
    int count = 0;
    for (int x = 0; x < size - 1; x++)
    for (int y = 0; y < size - 1; y++) {
        uint16_t i0 = (y + 0) * size + (x + 0);
        uint16_t i1 = (y + 1) * size + (x + 0);
        uint16_t i2 = (y + 1) * size + (x + 1);
        uint16_t i3 = (y + 0) * size + (x + 1);
        
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
            sizeof(uint16_t) * numIndices
        )
    );
    delete[] terrainIndices;
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

void ResourceManager::LoadVertexShader(const std::string& name) {
    VertexShader& shader = vertexShaders_.Add(name);
    ASSIGN_DEBUG_NAME(shader, name);
    std::string path = "./shaders/" + name + ".bin";
    MEMORYFROMFILE(path);
    if (memory == nullptr)
        abort();

    shader.handle = bgfx::createShader(memory);
    DEBUGLOG("Loaded vertex shader " << name);
}
void ResourceManager::UnloadVertexShader(const std::string& name) {
    bgfx::destroy(vertexShaders_.Get(name).handle);
    vertexShaders_.Remove(name);
}

void ResourceManager::LoadFragmentShader(const std::string& name) {
    FragmentShader& shader = fragmentShaders_.Add(name);
    ASSIGN_DEBUG_NAME(shader, name);
    std::string path = "./shaders/" + name + ".bin";
    MEMORYFROMFILE(path);
    if (memory == nullptr)
        abort();

    shader.handle = bgfx::createShader(memory);
    DEBUGLOG("Loaded fragment shader " << name);
}
void ResourceManager::UnloadFragmentShader(const std::string& name) {
    bgfx::destroy(fragmentShaders_.Get(name).handle);
    fragmentShaders_.Remove(name);
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
    textures_.Remove(name);
}

void ResourceManager::LoadMaterial(const std::string& name) {
    Material& material = materials_.Add(name);
    ASSIGN_DEBUG_NAME(material, name);
    std::ifstream inFile("materials/" + name + ".json");
    ASSERT(inFile.is_open(), "Failed to load material " + name);
    nlohmann::json data = nlohmann::json::parse(inFile);

    material.triangleType = ONE_SIDED;
    if (GetBoolean(data, "two_sided"))
        material.triangleType = TWO_SIDED;
    if (GetBoolean(data, "negative_back") && material.triangleType == TWO_SIDED)
        material.triangleType = TWO_SIDED_NEGATIVE_BACK;

    VertexShader* vertexShader = GetVertexShader(GetString(data, "vertex"));
    FragmentShader* fragmentShader = GetFragmentShader(GetString(data, "fragment"));
    material.shaderHandle = bgfx::createProgram(vertexShader->handle, fragmentShader->handle);

    material.castShadows = GetBoolean(data, "cast_shadows");
    if (material.castShadows) {
        VertexShader* vertexShadowShader = GetVertexShader(GetString(data, "vertex_shadow"));
        FragmentShader* fragmentShadowShader = GetFragmentShader(GetString(data, "fragment_shadow"));
        material.shadowShaderHandle = bgfx::createProgram(vertexShadowShader->handle, fragmentShadowShader->handle);
    }

    if (data.contains("textures")) {
        auto& textureNames = data["textures"];
        ASSERT((textureNames.size() <= Material::MAX_TEXTURES_PER_MATERIAL), "Too many textures on material " + name);
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

    #ifdef _DEBUG
    Material& selectedMaterial = materials_.Add(name + "_selected");
    selectedMaterial = material;
    selectedMaterial.shaderHandle = bgfx::createProgram(vertexShader->handle, GetFragmentShader("fs_selected")->handle); 
    #endif
    DEBUGLOG("Loaded material " << name);
}
void ResourceManager::UnloadMaterial(const std::string& name) {
    materials_.Remove(name);
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
    
    skeletons_.Remove(name);
}

void ResourceManager::LoadEntityDescription(const std::string& name) {
    EntityDescription& description = entityDescs_.Add(name);
    ASSIGN_DEBUG_NAME(description, name);

    std::ifstream inFile("entities/" + name + ".json");
    ASSERT(inFile.is_open(), "Failed to load entity " + name);

    description.data = nlohmann::json::parse(inFile);

    #ifdef _DEBUG
    description.DBG_name = name;
    #endif

    DEBUGLOG("Loaded entity " << name);
}
void ResourceManager::UnloadEntityDescription(const std::string& name) {
    entityDescs_.Remove(name);
}

void ResourceManager::LoadEmitterProperties(const std::string& name) {
    EmitterProperties& properties = emitterProps_.Add(name);
    ASSIGN_DEBUG_NAME(properties, name);

    std::ifstream inFile("emitters/" + name + ".json");
    ASSERT(inFile.is_open(), "Failed to load emitter" + name);

    nlohmann::json data = nlohmann::json::parse(inFile);

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

    DEBUGLOG("Loaded emitter " << name);
}
void ResourceManager::UnloadEmitterProperties(const std::string& name) {
    emitterProps_.Remove(name);
}
