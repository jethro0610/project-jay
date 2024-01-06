#include "EditorLevel.h"
#include "Level/LevelLoader.h"
#include "Level/LevelProperties.h"
#include "Terrain/Terrain.h"
#include <nlohmann/json.hpp>

EditorLevel::EditorLevel(
    EntityList& entities,
    LevelLoader& levelLoader,
    LevelProperties& properties,
    ResourceManager& resourceManager,
    Terrain& terrain
):
entities_(entities),
levelLoader_(levelLoader),
properties_(properties),
resourceManager_(resourceManager),
terrain_(terrain)
{
    name_ = ""; 
    hasLevel_ = false;
}

void EditorLevel::Save(const std::string& name, bool setAsNewLevel) {
    // assert(hasLevel_);
    // nlohmann::json level;
    //
    // level["spread"]["model"] = properties_.spreadModel->DBG_name;
    // for (Material* material : properties_.spreadMaterials)
    //     level["spread"]["materials"].push_back(material->DBG_name);
    //
    // level["terrain"]["material"] = properties_.terrainMaterial->DBG_name;
    // level["seed"]["material"] = properties_.seedMaterial->DBG_name;
    // 
    // level["blob"]["seed"] = properties_.blob.seed;
    // level["blob"]["frequency"] = properties_.blob.frequency;
    // level["blob"]["minRadius"] = properties_.blob.minRadius;
    // level["blob"]["maxRadius"] = properties_.blob.maxRadius;
    //
    // for (int i = 0; i < NoiseLayer::MAX; i++) {
    //     nlohmann::json noiseLayerData;
    //     noiseLayerData["active"] = properties_.noiseLayers[i].active;
    //     noiseLayerData["seed"] = properties_.noiseLayers[i].seed;
    //     noiseLayerData["frequency"]["x"] = properties_.noiseLayers[i].frequency.x;
    //     noiseLayerData["frequency"]["y"] = properties_.noiseLayers[i].frequency.y;
    //     noiseLayerData["multiplier"] = properties_.noiseLayers[i].multiplier;
    //     noiseLayerData["exponent"] = properties_.noiseLayers[i].exponent;
    //     level["noise_layers"][i] = noiseLayerData;
    // }
    // 
    // TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();
    // for (int i = 0; i < MAX_ENTITIES; i++) {
    //     const Entity& entity = entityManager_.entities_[i];
    //     if (!entity.alive_) continue;
    //     
    //     nlohmann::json entityData; 
    //     entityData["name"] = entity.DBG_name;
    //     Transform& transform = transformComponent.transform[i];
    //
    //     entityData["transform"]["position"]["x"] = transform.position.x;
    //     entityData["transform"]["position"]["y"] = transform.position.y;
    //     entityData["transform"]["position"]["z"] = transform.position.z;
    //
    //     entityData["transform"]["scale"]["x"] = transform.scale.x;
    //     entityData["transform"]["scale"]["y"] = transform.scale.y;
    //     entityData["transform"]["scale"]["z"] = transform.scale.z;
    //
    //     glm::vec3 eulerRotation = glm::eulerAngles(transform.rotation);
    //     entityData["transform"]["rotation"]["x"] = eulerRotation.x;
    //     entityData["transform"]["rotation"]["y"] = eulerRotation.y;
    //     entityData["transform"]["rotation"]["z"] = eulerRotation.z;
    //
    //     level["entities"].push_back(entityData);
    // }
    // std::ofstream assetLevelFile("../Assets/levels/" + name + ".json");
    // assetLevelFile << std::setw(4) << level << std::endl;
    // assetLevelFile.close();
    //
    // std::ofstream workingLevelFile("levels/" + name + ".json");
    // workingLevelFile<< std::setw(4) << level << std::endl;
    // workingLevelFile.close();
    //
    // if (setAsNewLevel)
    //     name_ = name;
    //
    // DEBUGLOG("Saved level: " << name);
}

void EditorLevel::New(const std::string& name) {
    name_ = name; 
    hasLevel_ = true;
    levelLoader_.ClearLevel();

    DependencyList deps = DependencyList::GenerateFromLevelProperties(
        "m_seed_test",
        "st_flower_test",
        { "m_flower_test", "m_stem_test" },
        "m_terrain_grass"
    );
    resourceManager_.UnloadUnusedDependencies(deps);
    resourceManager_.LoadDependencies(deps);
    properties_.seedMaterial = resourceManager_.GetMaterial("m_seed_test");
    properties_.spreadModel = resourceManager_.GetModel("st_flower_test");
    properties_.spreadMaterials.push_back(resourceManager_.GetMaterial("m_flower_test"));
    properties_.spreadMaterials.push_back(resourceManager_.GetMaterial("m_stem_test"));
    properties_.terrainMaterial = resourceManager_.GetMaterial("m_terrain_grass");

    // TODO: Assign other level properties
    for (NoiseLayer& noiseLayer : properties_.noiseLayers)
        noiseLayer = {};
    properties_.blob = {};

    properties_.seedMaterial = resourceManager_.GetMaterial("m_seed_test");

    terrain_.GenerateTerrainMap(properties_.noiseLayers, properties_.blob);
}

bool EditorLevel::Load(const std::string& name) {
    if (levelLoader_.LoadLevel(name)) {
        name_ = name;
        hasLevel_ = true;
        return true;
    }
    return false;
}

void EditorLevel::Reset(const std::string& resetSuffix) {
    assert(hasLevel_);
    levelLoader_.LoadLevel(name_ + resetSuffix, false);
}
