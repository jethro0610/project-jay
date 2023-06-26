#pragma once
#include <json.hpp>
#include <unordered_map>

class ResourceManager {
public:
    ResourceManager();
    std::unordered_map<std::string, nlohmann::json> entities_;
    void LoadEntity(std::string entityName);
};
