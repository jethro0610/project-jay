#pragma once
#include "Entity/TypeID.h"
#include <set>
#include <string>
#include <vector_const.h>
#include <nlohmann/json.hpp>

class Entity;

struct DependencyList {
    std::set<std::string> textures;
    std::set<std::string> models;

    static void GetFromEntity(TypeID typeId, DependencyList& outDepList);
    static void GetFromLevelJson(nlohmann::json& levelData, DependencyList& outDepList);
};
