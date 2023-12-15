#include "LevelLoader.h"
#include "Helpers/LoadHelpers.h"
#include "Entity/EntityManager.h"
#include "Spread/SpreadManager.h"
#include "Seed/SeedManager.h"
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
    SpreadManager& spreadManager
) :
entityManager_(entityManager),
levelProperties_(levelProperties),
particleManager_(particleManager),
resourceManager_(resourceManager),
seedManager_(seedManager),
spreadManager_(spreadManager)
{
    
}

void LevelLoader::LoadLevel(const std::string& name) {
    ClearLevel();

    #ifdef _DEBUG
    DBG_levelName_ = name;
    #endif

    std::ifstream inFile("levels/" + name + ".json");
    ASSERT(inFile.is_open(), "Failed to load level " + name);
    nlohmann::json levelData_ = nlohmann::json::parse(inFile);

    DependencyList dependencies = GenerateDepedencyList(levelData_);
    resourceManager_.UnloadUnusedDependencies(dependencies);
    resourceManager_.LoadDependencies(dependencies);
    
    levelProperties_.spreadModel = resourceManager_.GetModel(levelData_["spread"]["model"]);
    levelProperties_.spreadMaterials.clear();
    for (auto& materialName : levelData_["spread"]["materials"])
        levelProperties_.spreadMaterials.push_back(resourceManager_.GetMaterial(materialName));

    levelProperties_.terrainMaterial = resourceManager_.GetMaterial(levelData_["terrain"]["material"]);
    levelProperties_.seedMaterial = resourceManager_.GetMaterial(levelData_["seed"]["material"]);

    auto& entitiesData = levelData_["entities"];
    Transform entityTransform;
    for (auto& entityData : entitiesData) {
        entityTransform = GetTransform(entityData, "transform");
        entityManager_.spawnList_.push_back({resourceManager_.GetEntityDescription(entityData["name"]), entityTransform});
    }
    entityManager_.SpawnEntities();
}

#ifdef _DEBUG
void LevelLoader::SaveLevel() {
    nlohmann::json level;

    level["spread"]["model"] = levelProperties_.spreadModel->DBG_name;
    for (Material* material : levelProperties_.spreadMaterials)
        level["spread"]["materials"].push_back(material->DBG_name);

    level["terrain"]["material"] = levelProperties_.terrainMaterial->DBG_name;
    level["seed"]["material"] = levelProperties_.seedMaterial->DBG_name;
    
    TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entityManager_.entities_[i];
        if (!entity.alive_) continue;
        
        nlohmann::json entityData; 
        entityData["name"] = entity.DBG_name;
        Transform& transform = transformComponent.transform[i];

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
    std::ofstream levelFile("../Assets/levels/" + DBG_levelName_ + ".json");
    levelFile << std::setw(4) << level << std::endl;
    DEBUGLOG("Saved level: " << DBG_levelName_);
}
#endif

void LevelLoader::ClearLevel() {
    entityManager_.Reset();
    particleManager_.Reset();
    spreadManager_.Reset();
    seedManager_.Reset();
}

void ParseVertexShader(const std::string& name, DependencyList& list) {
    if (list.vertexShaders.contains(name)) return;
    list.vertexShaders.insert(name);
}

void ParseFragmentShader(const std::string& name, DependencyList& list) {
    if (list.fragmentShaders.contains(name)) return;
    list.fragmentShaders.insert(name);
}

void ParseMaterial(const std::string& name, DependencyList& list) {
    if (list.materials.contains(name)) return;
    list.materials.insert(name);
    std::ifstream materialFile("materials/" + name + ".json");
    nlohmann::json materialData = nlohmann::json::parse(materialFile);

    ParseVertexShader(materialData["vertex"].get<std::string>(), list);
    ParseFragmentShader(materialData["fragment"].get<std::string>(), list);

    if (materialData.contains("vertex_shadow"))
        ParseVertexShader(materialData["vertex_shadow"].get<std::string>(), list);
    if (materialData.contains("fragment_shadow"))
        ParseFragmentShader(materialData["fragment_shadow"].get<std::string>(), list);

    list.materials.insert(name);
    for (auto& textureName : materialData["textures"])
        list.textures.insert(textureName);
}

void ParseMaterials(nlohmann::json& materialNames, DependencyList& list) {
    for (auto& materialName : materialNames)
        ParseMaterial(materialName, list);
}

void ParseEmitterProperty(const std::string& name, DependencyList& list) {
    if (list.emitterProperties.contains(name)) return;
    list.emitterProperties.insert(name);    
    std::ifstream emitterFile("emitters/" + name + ".json");
    nlohmann::json emitterData = nlohmann::json::parse(emitterFile);

    ParseMaterial(emitterData["material"].get<std::string>(), list);
}

void ParseEmitterProperties(nlohmann::json& emitterNames, DependencyList& list) {
    for (auto& emitterName : emitterNames)
        ParseEmitterProperty(emitterName, list);
}

void ParseModel(const std::string& name, DependencyList& list) {
    if (list.models.contains(name)) return;
    list.models.insert(name);
}

void ParseEntity(const std::string& name, DependencyList& list) {
    if (list.entityDescriptions.contains(name)) return;
    list.entityDescriptions.insert(name);
    std::ifstream entityFile("entities/" + name + ".json");
    nlohmann::json entityData = nlohmann::json::parse(entityFile);

    if (entityData.contains("emitters"))
        ParseEmitterProperties(entityData["emitters"], list);

    if (entityData["components"].contains("static_model")) {
        auto& modelData = entityData["components"]["static_model"];
        ParseModel(modelData["model"].get<std::string>(), list);
        ParseMaterials(modelData["materials"], list);
    }

    if (entityData["components"].contains("interval_spawner")) {
        auto& spawnerData = entityData["components"]["interval_spawner"];
        ParseEntity(spawnerData["entity"], list);
    }

}

void ParseLevelEntities(nlohmann::json& entitiesData, DependencyList& list) {
    for (auto& entityData : entitiesData) {
        ParseEntity(entityData["name"].get<std::string>(), list);
    }
}

DependencyList LevelLoader::GenerateDepedencyList(nlohmann::json& levelData) {
    DependencyList list;
    ParseModel(levelData["spread"]["model"], list);
    ParseMaterials(levelData["spread"]["materials"], list);
    ParseMaterial(levelData["terrain"]["material"], list);
    ParseMaterial(levelData["seed"]["material"], list);
    ParseLevelEntities(levelData["entities"], list);
    return list;
}
