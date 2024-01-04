#include "LevelLoader.h"
#include "Helpers/LoadHelpers.h"
#include "Entity/EntityManager.h"
#include "Spread/SpreadManager.h"
#include "Seed/SeedManager.h"
#include "Terrain/Terrain.h"
#include "Logging/Logger.h"
#ifdef _DEBUG
#include <fstream>
#endif

LevelLoader::LevelLoader(
    EntityManager& entityManager, 
    LevelProperties& levelProperties,
    ParticleManager& particleManager,
    ResourceManager& resourceManager,
    SeedManager& seedManager,
    SpreadManager& spreadManager,
    Terrain& terrain
) :
entityManager_(entityManager),
levelProperties_(levelProperties),
particleManager_(particleManager),
resourceManager_(resourceManager),
seedManager_(seedManager),
spreadManager_(spreadManager),
terrain_(terrain)
{
    
}

bool LevelLoader::LoadLevel(const std::string& name, bool loadTerrain) {
    std::ifstream inFile("levels/" + name + ".json");
    if (!inFile.is_open())
        return false;

    ClearLevel();
    nlohmann::json levelData = nlohmann::json::parse(inFile);

    DependencyList deps = DependencyList::GenerateFromLevel(levelData);
    resourceManager_.UnloadUnusedDependencies(deps);
    resourceManager_.LoadDependencies(deps);
    
    levelProperties_.spreadModel = resourceManager_.GetModel(levelData["spread"]["model"]);
    levelProperties_.spreadMaterials.clear();
    for (auto& materialName : levelData["spread"]["materials"])
        levelProperties_.spreadMaterials.push_back(resourceManager_.GetMaterial(materialName));

    levelProperties_.terrainMaterial = resourceManager_.GetMaterial(levelData["terrain"]["material"]);
    levelProperties_.seedMaterial = resourceManager_.GetMaterial(levelData["seed"]["material"]);

    if (levelData.contains("blob")) {
        levelProperties_.blob.seed = levelData["blob"]["seed"];
        levelProperties_.blob.frequency = levelData["blob"]["frequency"];
        levelProperties_.blob.minRadius = levelData["blob"]["minRadius"];
        levelProperties_.blob.maxRadius = levelData["blob"]["maxRadius"];
    }

    if (levelData.contains("noise_layers")) {
        for (int i = 0; i < NoiseLayer::MAX; i++) {
            nlohmann::json& noiseLayerData = levelData["noise_layers"][i];
            levelProperties_.noiseLayers[i].active = noiseLayerData["active"];
            levelProperties_.noiseLayers[i].seed = noiseLayerData["seed"];
            levelProperties_.noiseLayers[i].frequency.x = noiseLayerData["frequency"]["x"];
            levelProperties_.noiseLayers[i].frequency.y = noiseLayerData["frequency"]["y"];
            levelProperties_.noiseLayers[i].multiplier = noiseLayerData["multiplier"];
            levelProperties_.noiseLayers[i].exponent = noiseLayerData["exponent"];
        }
    }

    if (loadTerrain)
        terrain_.GenerateTerrainMap(levelProperties_.noiseLayers, levelProperties_.blob);

    auto& entitiesData = levelData["entities"];
    Transform entityTransform;
    for (auto& entityData : entitiesData) {
        entityTransform = GetTransform(entityData, "transform");
        entityManager_.spawner_.Spawn(resourceManager_.GetEntityDescription(entityData["name"]), entityTransform);
    }
    entityManager_.SpawnEntities();

    return true;
}

void LevelLoader::ClearLevel() {
    entityManager_.Reset();
    // particleManager_.Reset();
    spreadManager_.Reset();
    seedManager_.Reset();
}
