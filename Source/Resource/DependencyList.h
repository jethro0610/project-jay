#pragma once
#include <set>
#include <string>

struct DependencyList {
    std::set<std::string> vertexShaders;
    std::set<std::string> fragmentShaders;
    std::set<std::string> textures;
    std::set<std::string> materials;
    std::set<std::string> models;
    std::set<std::string> emitterProperties;
    std::set<std::string> entityDescriptions;
};
