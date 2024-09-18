#include "DependencyList.h"
#include "Logging/Logger.h"
#include "Entity/Entity.h"
#include <nlohmann/json.hpp>

void DependencyList::GetFromEntity(Entity::TypeID typeId, DependencyList& outDepList) {
    EntityDependendies deps = Entity::GetDependencies(typeId);
    if (deps.hasModel)
        outDepList.models.insert(deps.model);
    for (int i = 0; i < deps.textures.size(); i++)
        outDepList.textures.insert(std::string(deps.textures[i]));
}

void DependencyList::GetFromLevelJson(nlohmann::json& levelData, DependencyList& outDepList) {
    auto& entitiesData = levelData["entities"];
    for (auto& entityData : entitiesData) {
        Entity::TypeID typeId;
        if (!entityData.contains("type_id")) {
            typeId = Entity::GetTypeIDFromName(entityData["name"]);
        }
        else
            typeId = entityData["type_id"];

        GetFromEntity(typeId, outDepList);
    }
}
