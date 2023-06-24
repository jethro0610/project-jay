#pragma once
#include <json.hpp>
#include <unordered_map>
#include "RawModel.h"

class ResourceManager {
public:
    ResourceManager();
    std::unordered_map<std::string, nlohmann::json> entities_;
    void LoadEntity(std::string entityName);
};
