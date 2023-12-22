#pragma once
#include <set>
#include <string>
#include <nlohmann/json.hpp>

struct DependencyList {
    std::set<std::string> vertexShaders;
    std::set<std::string> fragmentShaders;
    std::set<std::string> textures;
    std::set<std::string> materials;
    std::set<std::string> models;
    std::set<std::string> emitterProperties;
    std::set<std::string> entityDescriptions;

    static DependencyList GenerateFromEntity(nlohmann::json& entityData);
    static DependencyList GenerateFromEntity(const std::string& name);
    static DependencyList GenerateFromLevel(nlohmann::json& levelData);
    static DependencyList GenerateFromLevel(const std::string& name);
};
