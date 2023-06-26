#include <fstream>
#include "ResourceManager.h"
using json = nlohmann::json;

void ResourceManager::LoadEntity(std::string entityName) {
    assert(entities_.count(entityName) == 0);
    std::ifstream inFile("entities/" + entityName + ".json");
    json data = json::parse(inFile);
    entities_[entityName] = data;
}
