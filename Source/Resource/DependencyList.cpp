#include "DependencyList.h"
#include "Helpers/LoadHelpers.h"
#include <nlohmann/json.hpp>
#include <fstream>

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
        if (GetBoolean(spawnerData, "seed") == false)
            ParseEntity(spawnerData["entity"], list);
    }

}

void ParseLevelEntities(nlohmann::json& entitiesData, DependencyList& list) {
    for (auto& entityData : entitiesData) {
        ParseEntity(entityData["name"].get<std::string>(), list);
    }
}

DependencyList DependencyList::GenerateFromEntity(nlohmann::json& entityData) {
    DependencyList list;
    ParseEntity(entityData, list);
    return list;
}

DependencyList DependencyList::GenerateFromEntity(const std::string& name) {
    std::ifstream entityFile("entities/" + name + ".json");
    nlohmann::json levelData = nlohmann::json::parse(entityFile);
    return GenerateFromLevel(levelData);
}

DependencyList DependencyList::GenerateFromLevel(nlohmann::json& levelData) {
    DependencyList list;
    ParseModel(levelData["spread"]["model"], list);
    ParseMaterials(levelData["spread"]["materials"], list);
    ParseMaterial(levelData["terrain"]["material"], list);
    ParseMaterial(levelData["seed"]["material"], list);
    ParseLevelEntities(levelData["entities"], list);
    return list;
}

DependencyList DependencyList::GenerateFromLevel(const std::string& name) {
    std::ifstream levelFile("levels/" + name + ".json");
    nlohmann::json levelData = nlohmann::json::parse(levelFile);
    return GenerateFromLevel(levelData);
}
