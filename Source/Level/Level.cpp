#include "Level.h"
#include "Helpers/LoadHelpers.h"
#include "Helpers/MapCheck.h"
#include "Spread/SpreadManager.h"
#include "Seed/SeedManager.h"
#include "Terrain/Terrain.h"
#include "Logging/Logger.h"
#include "Entity/EntityList.h"
#include "Particle/ParticleManager.h"
#include "Entity/EntityTypes.h"
#include "Resource/ResourceManager.h"
#ifdef _DEBUG
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
    #define ENTITYEXP(TYPE, VAR) entityIds_[TYPE::GetName()] = TYPE::GetTypeID();
    EXPANDENTITIES
    #undef ENTITYEXP
    loaded_ = false;
}

bool Level::Load(const std::string& name, const std::string& suffix, bool loadTerrain) {
    std::ifstream inFile("levels/" + name + suffix + ".json");
    if (!inFile.is_open())
        return false;

    Clear();
    nlohmann::json levelData = nlohmann::json::parse(inFile);

    DependencyList deps = DependencyList::GenerateFromLevel(levelData);
    resourceManager_.UnloadUnusedDependencies(deps);
    resourceManager_.LoadDependencies(deps);
    
    properties_.spreadModel = resourceManager_.GetModel(levelData["spread"]["model"]);
    properties_.spreadMaterials.clear();
    for (auto& materialName : levelData["spread"]["materials"])
        properties_.spreadMaterials.push_back(resourceManager_.GetMaterial(materialName));

    properties_.terrainMaterial = resourceManager_.GetMaterial(levelData["terrain"]["material"]);
    properties_.seedMaterial = resourceManager_.GetMaterial(levelData["seed"]["material"]);

    if (levelData.contains("blob")) {
        properties_.blob.seed = levelData["blob"]["seed"];
        properties_.blob.frequency = levelData["blob"]["frequency"];
        properties_.blob.minRadius = levelData["blob"]["minRadius"];
        properties_.blob.maxRadius = levelData["blob"]["maxRadius"];
    }

    if (levelData.contains("noise_layers")) {
        for (int i = 0; i < NoiseLayer::MAX; i++) {
            nlohmann::json& noiseLayerData = levelData["noise_layers"][i];
            properties_.noiseLayers[i].active = noiseLayerData["active"];
            properties_.noiseLayers[i].seed = noiseLayerData["seed"];
            properties_.noiseLayers[i].frequency.x = noiseLayerData["frequency"]["x"];
            properties_.noiseLayers[i].frequency.y = noiseLayerData["frequency"]["y"];
            properties_.noiseLayers[i].multiplier = noiseLayerData["multiplier"];
            properties_.noiseLayers[i].exponent = noiseLayerData["exponent"];
        }
    }

    if (loadTerrain)
        terrain_.GenerateTerrainMap(properties_.noiseLayers, properties_.blob);

    auto& entitiesData = levelData["entities"];
    Transform entityTransform;
    for (auto& entityData : entitiesData) {
        Entity::TypeID type = GetFromMap<Entity::TypeID>(
            entityIds_, 
            entityData["name"], 
            "entity " + entityData["name"].get<std::string>() + " not found"
        ); 
        Entity& entity = entities_.CreateEntity(type);
        entity.transform_ = GetTransform(entityData, "transform");
    }
    DBG_name_ = name;
    loaded_ = true;
    return true;
}

#ifdef _DEBUG
void Level::Save(const std::string& name, const std::string& suffix) {
    nlohmann::json level;

    level["spread"]["model"] = properties_.spreadModel->DBG_name;
    for (Material* material : properties_.spreadMaterials)
        level["spread"]["materials"].push_back(material->DBG_name);

    level["terrain"]["material"] = properties_.terrainMaterial->DBG_name;
    level["seed"]["material"] = properties_.seedMaterial->DBG_name;

    level["blob"]["seed"] = properties_.blob.seed;
    level["blob"]["frequency"] = properties_.blob.frequency;
    level["blob"]["minRadius"] = properties_.blob.minRadius;
    level["blob"]["maxRadius"] = properties_.blob.maxRadius;

    for (int i = 0; i < NoiseLayer::MAX; i++) {
        nlohmann::json noiseLayerData;
        noiseLayerData["active"] = properties_.noiseLayers[i].active;
        noiseLayerData["seed"] = properties_.noiseLayers[i].seed;
        noiseLayerData["frequency"]["x"] = properties_.noiseLayers[i].frequency.x;
        noiseLayerData["frequency"]["y"] = properties_.noiseLayers[i].frequency.y;
        noiseLayerData["multiplier"] = properties_.noiseLayers[i].multiplier;
        noiseLayerData["exponent"] = properties_.noiseLayers[i].exponent;
        level["noise_layers"][i] = noiseLayerData;
    }

    for (int i = 0; i < 128; i++) {
        Entity& entity = entities_[i];
        if (!entity.alive_) continue;
        
        nlohmann::json entityData; 
        entityData["name"] = entity.DBG_name_;
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

        level["entities"].push_back(entityData);
    }
    std::ofstream assetLevelFile("../Assets/levels/" + name + suffix + ".json");
    assetLevelFile << std::setw(4) << level << std::endl;
    assetLevelFile.close();

    std::ofstream workingLevelFile("levels/" + name + suffix + ".json");
    workingLevelFile<< std::setw(4) << level << std::endl;
    workingLevelFile.close();

    DEBUGLOG("Saved level: " << name + suffix);
}
#endif

void Level::Clear() {
    entities_.Reset();
    particleManager_.Reset();
    spreadManager_.Reset();
    seedManager_.Reset();
}
