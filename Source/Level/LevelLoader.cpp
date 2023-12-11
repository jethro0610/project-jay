#include "LevelLoader.h"
#include "Helpers/LoadHelpers.h"
#include "Entity/EntityManager.h"
#include "Logging/Logger.h"

LevelLoader::LevelLoader(ResourceManager& resourceManager, EntityManager& entityManager) :
resourceManager_(resourceManager),
entityManager_(entityManager)
{
    
}

void LevelLoader::UnloadLevel() {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entityManager_.entities_[i].alive_)
            continue;

        entityManager_.destroyList_.push_back({i, false});
    }
    entityManager_.DestroyEntities();
}

void LevelLoader::LoadLevel(const std::string& name) {

    std::ifstream inFile("levels/" + name + ".json");
    ASSERT(inFile.is_open(), "Failed to load level " + name);
    nlohmann::json data = nlohmann::json::parse(inFile);
    
    DependencyList dependencies = GenerateDepedencyList(data);
    resourceManager_.UnloadUnusedDependencies(dependencies);
    resourceManager_.LoadDependencies(dependencies);

    UnloadLevel();
    auto& entitiesData = data["entities"];
    Transform entityTransform;
    for (auto& entityData : entitiesData) {
        entityTransform = GetTransform(entityData, "transform");
        entityManager_.spawnList_.push_back({resourceManager_.GetEntityDescription(entityData["name"]), entityTransform});
    }
    entityManager_.SpawnEntities();

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

    if (entityData["components"].contains("static_model")) {
        auto& modelData = entityData["components"]["static_model"];
        ParseModel(modelData["model"].get<std::string>(), list);
        ParseMaterials(modelData["materials"], list);
    }
}

void ParseLevelEntities(nlohmann::json& entitiesData, DependencyList& list) {
    for (auto& entityData : entitiesData)
        ParseEntity(entityData["name"].get<std::string>(), list);
}

DependencyList LevelLoader::GenerateDepedencyList(nlohmann::json& levelData) {
    DependencyList list;
    ParseLevelEntities(levelData["entities"], list);
    return list;
}
