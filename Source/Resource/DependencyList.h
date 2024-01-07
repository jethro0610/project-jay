#pragma once
#include "Rendering/Model.h"
#include <set>
#include <string>
#include <nlohmann/json.hpp>
#include <vector_const.h>

struct DependencyList {
    std::set<std::string> textures;
    std::set<std::string> models;
};
