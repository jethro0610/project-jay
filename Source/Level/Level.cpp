#include "Level.h"
#include "Helpers/LoadHelpers.h"
#include "Helpers/MapCheck.h"
#include "Spread/SpreadManager.h"
#include "Seed/SeedManager.h"
#include "Terrain/Terrain.h"
#include "Logging/Logger.h"
#include "Entity/EntityList.h"
#include "Particle/ParticleManager.h"
#include "Resource/ResourceManager.h"
#ifdef _DEBUG
#include "Entity/EntityTypes.h"
#include <fstream>
#endif

Level::Level(
    EntityList& entities,
    ParticleManager& particleManager,
    ResourceManager& resourceManager,
    SeedManager& seedManager,
    SpreadManager& spreadManager,
    Terrain& terrain
) :
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
    properties_.terrainMaterial.textures[0] = resourceManager.GetTexture("t_grass_c");
    properties_.terrainMaterial.textures[1] = resourceManager.GetTexture("t_grass_n");
    properties_.terrainMaterial.textures[2] = resourceManager.GetTexture("t_marble_c");
    properties_.terrainMaterial.properties.fresnelPower = 16.0f;
    properties_.terrainMaterial.properties.fresnelScale = 1.0f;
    properties_.terrainMaterial.properties.fresnelBrightness = 1.0f;

    properties_.seedMaterial.shader = resourceManager.GetShader("vs_inst_billboard", "fs_seed_test");
    properties_.seedMaterial.castShadows = false;

    properties_.spreadModel = resourceManager.GetModel("st_flower_test");

    properties_.spreadMaterials[0].shader = resourceManager.GetShader("vs_spread", "fs_flower_test");
    properties_.spreadMaterials[0].shadowShader = resourceManager.GetShader("vs_inst_s", "fs_depth_masked_s");
    properties_.spreadMaterials[0].castShadows = true;
    properties_.spreadMaterials[0].numTextures = 1;
    properties_.spreadMaterials[0].textures[0] = resourceManager.GetTexture("t_flower_m");
    properties_.spreadMaterials[0].triangleType = TriangleType::TWO_SIDED;

    properties_.spreadMaterials[1].shader = resourceManager.GetShader("vs_spread", "fs_dfsa_color");
    properties_.spreadMaterials[1].shadowShader = resourceManager.GetShader("vs_inst_s", "fs_depth_s");
    properties_.spreadMaterials[1].numTextures = 0;
    properties_.spreadMaterials[1].properties.color = glm::vec4(0.85f, 1.0f, 0.5f, 1.0f);

    #ifdef _DEBUG
    #define ENTITYEXP(TYPE, VAR, ID) DBG_entityTypes_[TYPE::GetName()] = ID;
    EXPANDENTITIES
    #undef ENTITYEXP
    #endif
}

bool Level::Load(const std::string& name, const std::string& suffix, bool loadTerrain) {
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
        
        for (auto& bubbleData : levelData["bubbles"]) {
            TerrainBubble bubble;
            bubble.destroy_ = false;
            bubble.DBG_selected_ = false;
            bubble.position = GetVec4(bubbleData);
            terrain_.DBG_bubbles_.push_back(bubble);
        }

        for (auto& curveData: levelData["DBG_curves"]) {
            TerrainCurve curve;
            curve.destroy_ = false;
            curve.DBG_selectedPoint_ = -1;

            for (int i = 0; i < 4; i++) {
                curve.points[i] = GetVec4(curveData[i]);
            }
            terrain_.DBG_curves_.push_back(curve);
        }
        terrain_.GenerateTerrainDistances();
        terrain_.GenerateTerrainHeights();
    }
    #endif

    for (int i = 0; i < MAX_PHASES; i++)
        phases_[i] = levelData["phases"][i];

    StartPhase();

    loaded_ = true;
    return true;
}

void Level::StartPhase() {
    phase_ = 0;
    Clear();
    SpawnEntitiesInPhase(phase_);
}

void Level::NextPhase() {
    if (phase_ >= MAX_PHASES - 1) {
        DEBUGLOG("Error: going to phase greater than max");
        return;
    }
    phase_++;

    // TODO: Only remove non-persistent entities
    for (int i = 0; i < 128; i++) {
        Entity& entity = entities_[i];
        if (entity.alive_ && !entity.persist_)
            entity.destroy_ = true;
    }
    entities_.DestroyFlaggedEntities();
    SpawnEntitiesInPhase(phase_);
}

void Level::SpawnEntitiesInPhase(int phase, bool persistView) {
    auto& entitiesData = phases_[phase];
    Transform entityTransform;
    for (auto& entityData : entitiesData) {
        if (phase != phase_ && !entityData["persist"]) continue;
        Entity* entity;
        entityTransform = GetTransform(entityData, "transform");

        #ifndef _DEBUG
        entity = &entities_.CreateEntity(entityData["type_id"], entityTransform);
        #else
        if (entityData.contains("type_id"))
            entity = &entities_.CreateEntity(entityData["type_id"], entityTransform);
        else if (DBG_entityTypes_.contains(entityData["name"]))
            entity = &entities_.CreateEntity(DBG_entityTypes_[entityData["name"]], entityTransform);
        else
            DEBUGLOG("Error: attempted to spawn non-existant entity with name " << entityData["name"]);
        entity->DBG_persistView_ = persistView;
        #endif
        entity->persist_ = entityData["persist"];
    }
}

#ifdef _DEBUG
void Level::SaveCurrentPhase() {
    phases_[phase_].clear();

    for (int i = 0; i < 128; i++) {
        Entity& entity = entities_[i];
        if (!entity.alive_ || entity.DBG_persistView_) continue;
        
        nlohmann::json entityData; 
        entityData["name"] = entity.DBG_name_;
        entityData["type_id"] = entity.typeId_;
        Transform& transform = entity.transform_;

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

        entityData["persist"] = entity.persist_;

        phases_[phase_].push_back(entityData);
    }
}

void Level::Save(const std::string& name, const std::string& suffix) {
    DBG_name_ = name;

    nlohmann::json levelData;
    levelData["landmap"] = terrain_.DBG_landMapName_;

    SaveCurrentPhase();
    for (int i = 0; i < MAX_PHASES; i++)
        levelData["phases"].push_back(phases_[i]);

    for (int i = 0; i < terrain_.DBG_bubbles_.size(); i++) {
        TerrainBubble& bubble = terrain_.DBG_bubbles_[i];
        nlohmann::json bubbleData;

        bubbleData["x"] = bubble.position.x;
        bubbleData["y"] = bubble.position.y;
        bubbleData["z"] = bubble.position.z;
        bubbleData["w"] = bubble.position.w;
        levelData["bubbles"].push_back(bubbleData);
    }

    for (int i = 0; i < terrain_.DBG_curves_.size(); i++) {
        TerrainCurve& curve = terrain_.DBG_curves_[i];
        nlohmann::json curveData;

        for (int j = 0; j < 4; j++) {
            nlohmann::json pointData;
            pointData["x"] = curve.points[j].x;
            pointData["y"] = curve.points[j].y;
            pointData["z"] = curve.points[j].z;
            pointData["w"] = curve.points[j].w;
            curveData.push_back(pointData);
        }
        levelData["DBG_curves"].push_back(curveData);
    }

    std::ofstream assetLevelFile("../Assets/levels/" + name + suffix + ".json");
    assetLevelFile << std::setw(4) << levelData << std::endl;
    assetLevelFile.close();

    std::ofstream workingLevelFile("levels/" + name + suffix + ".json");
    workingLevelFile<< std::setw(4) << levelData << std::endl;
    workingLevelFile.close();

    DEBUGLOG("Saved level: " << name + suffix);
}

void Level::EditorSwitchPhase(int phase) {
    SaveCurrentPhase();
    phase_ = phase;

    entities_.Reset();
    SpawnEntitiesInPhase(phase_, false);
    if (DBG_persistView_) {
        for (int i = 0; i < phase_; i++)
            SpawnEntitiesInPhase(i, true);
    }
}

void Level::TogglePersistView() {
    DBG_persistView_ = !DBG_persistView_;
    EditorSwitchPhase(phase_);
}
#endif

void Level::Clear(bool clearTerrain) {
    entities_.Reset();
    particleManager_.Reset();
    spreadManager_.Reset();
    seedManager_.Reset();

    #ifdef _DEBUG
    if (clearTerrain) {
        terrain_.DBG_bubbles_.clear();
        terrain_.DBG_curves_.clear();
        terrain_.GenerateTerrainHeights();
    }
    #endif 
}
