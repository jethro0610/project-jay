#include "Level.h"
#include "Game/Clock.h"
#include "Helpers/LoadHelpers.h"
#include "Helpers/MapCheck.h"
#include "Spread/SpreadManager.h"
#include "Seed/SeedManager.h"
#include "Terrain/Terrain.h"
#include "Logging/Logger.h"
#include "Entity/Entity.h"
#include "Entity/EntityList.h"
#include "Particle/ParticleManager.h"
#include "Resource/ResourceManager.h"
#ifdef _DEBUG
#include <fstream>
#endif

Level::Level(
    Clock& clock,
    EntityList& entities,
    ParticleManager& particleManager,
    ResourceManager& resourceManager,
    SeedManager& seedManager,
    SpreadManager& spreadManager,
    Terrain& terrain
) :
    clock_(clock),
    entities_(entities),
    particleManager_(particleManager),
    resourceManager_(resourceManager),
    seedManager_(seedManager),
    spreadManager_(spreadManager),
    terrain_(terrain)
{
    loaded_ = false;

    properties_.terrainMaterial.shader = resourceManager.GetShader("vs_terrain", "fs_terrain");
    properties_.terrainMaterial.numTextures = 3;
    properties_.terrainMaterial.triangleType = TriangleType::TWO_SIDED_NEGATIVE_BACK;
    properties_.terrainMaterial.textures[0] = resourceManager.GetTexture("t_grass_c");
    properties_.terrainMaterial.textures[1] = resourceManager.GetTexture("t_grass_n");
    properties_.terrainMaterial.textures[2] = resourceManager.GetTexture("t_marble_c");

    properties_.seedMaterial.shader = resourceManager.GetShader("vs_inst_billboard", "fs_seed");
    properties_.seedMaterial.shadowShader = resourceManager.GetShader("vs_inst_billboard_s", "fs_seed_s");
    properties_.seedMaterial.castShadows = true;
    properties_.seedMaterial.transparencyType = TRANSPARENCY_UNORDERED;

    properties_.spreadModel = resourceManager.GetModel("st_flower_test");
    properties_.weedModel_ = resourceManager.GetModel("st_flower_test");

    properties_.spreadMaterials[SpreadType_Flower][0].shader = resourceManager.GetShader("vs_spread_activeonly", "fs_flower_test");
    properties_.spreadMaterials[SpreadType_Flower][0].shadowShader = resourceManager.GetShader("vs_spread_activeonly_s", "fs_depth_masked_s");
    properties_.spreadMaterials[SpreadType_Flower][0].castShadows = true;
    properties_.spreadMaterials[SpreadType_Flower][0].numTextures = 1;
    properties_.spreadMaterials[SpreadType_Flower][0].textures[0] = resourceManager.GetTexture("t_flower_m");
    properties_.spreadMaterials[SpreadType_Flower][0].triangleType = TriangleType::TWO_SIDED;

    properties_.spreadMaterials[SpreadType_Flower][1].shader = resourceManager.GetShader("vs_spread", "fs_dfsa_color");
    properties_.spreadMaterials[SpreadType_Flower][1].shadowShader = resourceManager.GetShader("vs_spread_s", "fs_depth_s");
    properties_.spreadMaterials[SpreadType_Flower][1].numTextures = 0;
    properties_.spreadMaterials[SpreadType_Flower][1].properties.color = glm::vec4(0.85f, 1.0f, 0.5f, 1.0f);

    properties_.spreadMaterials[SpreadType_Weed][0].shader = resourceManager.GetShader("vs_spread", "fs_dfsa");
    properties_.spreadMaterials[SpreadType_Weed][0].shadowShader = resourceManager.GetShader("vs_spread_s", "fs_depth_masked_s");
    properties_.spreadMaterials[SpreadType_Weed][0].castShadows = true;
    properties_.spreadMaterials[SpreadType_Weed][0].numTextures = 1;
    properties_.spreadMaterials[SpreadType_Weed][0].textures[0] = resourceManager.GetTexture("t_flower_m");
    properties_.spreadMaterials[SpreadType_Weed][0].triangleType = TriangleType::TWO_SIDED;

    properties_.spreadMaterials[SpreadType_Weed][1].shader = resourceManager.GetShader("vs_spread", "fs_dfsa_color");
    properties_.spreadMaterials[SpreadType_Weed][1].shadowShader = resourceManager.GetShader("vs_spread_s", "fs_depth_s");
    properties_.spreadMaterials[SpreadType_Weed][1].numTextures = 0;
    properties_.spreadMaterials[SpreadType_Weed][1].properties.color = glm::vec4(0.85f, 1.0f, 0.5f, 1.0f);
}

bool Level::Load(const std::string& name, const std::string& suffix, bool loadTerrain, bool editorLoad) {
    std::ifstream inFile("levels/" + name + suffix + ".json");
    if (!inFile.is_open())
        return false;

    Clear(loadTerrain);
    nlohmann::json levelData = nlohmann::json::parse(inFile);

    DependencyList deps; 
    DependencyList::GetFromLevelJson(levelData, deps);
    resourceManager_.UnloadUnusedDependencies(deps);
    resourceManager_.LoadDependencies(deps);

    #ifdef _DEBUG
    DBG_name_ = name;

    if (loadTerrain) {
        if (levelData.contains("landmap"))
            terrain_.DBG_landMapName_ = levelData["landmap"];
        else
            terrain_.DBG_landMapName_ = "lm_default";

        for (auto& modifierJson : levelData["terrain_modifiers"]) {
            int id = modifierJson["id"];
            StaticTerrainModifier& modifier = terrain_.CreateStaticModifier(id);
            modifier.Load(modifierJson);
        }

        terrain_.GenerateTerrainDistances();
        terrain_.GenerateTerrainHeights();
    }
    #endif

    for (auto& entityData : levelData["entities"]) {
        Entity* entity;
        Transform entityTransform = GetTransform(entityData, "transform");
        if (entityData.contains("floor_dist") && !entityData["floor_dist"].is_null()) {
            float floorDist = entityData["floor_dist"];
            entityTransform.position.y = terrain_.GetDistance(entityTransform.position).y + floorDist;
        }

        #ifndef _DEBUG
        entity = &entities_.CreateEntity(entityData["type_id"], entityTransform, editorLoad);
        #else
        TypeID typeIdFromName = entity->GetTypeIDFromName(entityData["name"]);
        if (entityData.contains("type_id"))
            entity = &entities_.CreateEntity(entityData["type_id"], entityTransform, editorLoad);
        else if (typeIdFromName != -1)
            entity = &entities_.CreateEntity(typeIdFromName, entityTransform, editorLoad);
        else
            DEBUGLOG("Error: attempted to spawn non-existant entity with name " << entityData["name"]);
        #endif

        EntityProperties properties = entity->GetProperties();

        if (entityData.contains("float_properties") && !entityData["float_properties"].is_null()) {
            for (auto& pair : properties.floats) {
                if (entityData["float_properties"][pair.first].is_null())
                    continue;

                *pair.second = entityData["float_properties"][pair.first];
            }
        }
        if (entityData.contains("int_properties") && !entityData["int_properties"].is_null()) {
            for (auto& pair : properties.ints) {
                if (entityData["int_properties"][pair.first].is_null())
                    continue;

                *pair.second = entityData["int_properties"][pair.first];
            }
        }
        if (entityData.contains("bool_properties") && !entityData["bool_properties"].is_null()) {
            for (auto& pair : properties.bools) {
                if (entityData["bool_properties"][pair.first].is_null())
                    continue;

                *pair.second = entityData["bool_properties"][pair.first];
            }
        }
    }

    for (auto& spreadData : levelData["spread"]) {
        int x = spreadData[0];
        int y = spreadData[1];
        SpreadType type = spreadData[2];
        spreadManager_.AddSpread(glm::ivec2(x, y), type);
    }

    loaded_ = true;
    return true;
}

#ifdef _DEBUG
void Level::Save(const std::string& name, const std::string& suffix) {
    DBG_name_ = name;

    nlohmann::json levelData;
    levelData["landmap"] = terrain_.DBG_landMapName_;

    for (int i = 0; i < EntityList::MAX; i++) {
        Entity& entity = entities_[i];
        if (!entity.alive_) continue;

        nlohmann::json entityData; 
        entityData["name"] = entity.GetName();
        entityData["type_id"] = entity.typeId_;
        Transform& transform = entity.transform_;

        glm::vec2 terrainDist = terrain_.GetDistance(entity.transform_.position);
        if (terrainDist.x < 0.0)
            entityData["floor_dist"] = transform.position.y - terrainDist.y;

        entityData["transform"]["position"]["x"] = transform.position.x;
        entityData["transform"]["position"]["y"] = transform.position.y;
        entityData["transform"]["position"]["z"] = transform.position.z;

        entityData["transform"]["scale"]["x"] = transform.scale.x;
        entityData["transform"]["scale"]["y"] = transform.scale.y;
        entityData["transform"]["scale"]["z"] = transform.scale.z;

        glm::vec3 eulerRotation = glm::eulerAngles(transform.rotation);
        entityData["transform"]["rotation"]["x"] = eulerRotation.x;
        entityData["transform"]["rotation"]["y"] = eulerRotation.y;
        entityData["transform"]["rotation"]["z"] = eulerRotation.z;

        EntityProperties properties = entity.GetProperties();

        nlohmann::json floatProperties;
        for (auto& pair : properties.floats)
            floatProperties[pair.first] = *pair.second;    
        entityData["float_properties"] = floatProperties;

        nlohmann::json intProperties;
        for (auto& pair : properties.ints)
            intProperties[pair.first] = *pair.second;    
        entityData["int_properties"] = intProperties;

        nlohmann::json boolProperties;
        for (auto& pair : properties.bools)
            boolProperties[pair.first] = *pair.second;    
        entityData["bool_properties"] = boolProperties;

        levelData["entities"].push_back(entityData);
    }


    for (StaticTerrainModifier* modifier : terrain_.DBG_modifiers_) {
        if (!modifier->active_)
            continue;

        nlohmann::json modifierJson;
        modifierJson["id"] = modifier->id_;
        modifierJson["name"] = modifier->GetName();
        modifier->Save(modifierJson);
        levelData["terrain_modifiers"].push_back(modifierJson);
    }

    for (int i = 0; i < SpreadManager::NUM_CHUNKS; i++) {
        SpreadManager::Chunk& chunk = spreadManager_.chunks_[i];
        if (!chunk.active)
            continue;

        for (int x = 0; x < SpreadManager::CHUNK_SIZE; x++) {
        for (int y = 0; y < SpreadManager::CHUNK_SIZE; y++) {
            if (chunk.indexes[x][y] == -1)
                continue;
            
            nlohmann::json spreadData;
            SpreadManager::ChunkSpacePosition chunkSpacePos(chunk.origin, glm::ivec2(x, y));
            glm::ivec2 spreadSpacePos = SpreadManager::ChunkSpaceToSpreadSpace(chunkSpacePos);
            spreadData.push_back(spreadSpacePos.x);    
            spreadData.push_back(spreadSpacePos.y);    
            spreadData.push_back(chunk.types[x][y]);    
            levelData["spread"].push_back(spreadData);
        }}
    }

    std::ofstream assetLevelFile("../Assets/levels/" + name + suffix + ".json");
    assetLevelFile << std::setw(4) << levelData << std::endl;
    assetLevelFile.close();

    std::ofstream workingLevelFile("levels/" + name + suffix + ".json");
    workingLevelFile<< std::setw(4) << levelData << std::endl;
    workingLevelFile.close();

    DEBUGLOG("Saved level: " << name + suffix);
}
#endif

void Level::Clear(bool clearTerrain) {
    entities_.Reset();
    particleManager_.Reset();
    spreadManager_.Reset();
    seedManager_.Reset();
    clock_.Reset();

    if (clearTerrain)
        terrain_.Reset();
}
