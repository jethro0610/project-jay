#include "DependencyList.h"
#include "Entity/EntityTypes.h"
#include "Level/Level.h"
#include "Logging/Logger.h"
#include <nlohmann/json.hpp>

void DependencyList::GetFromEntity(Entity::TypeID typeId, DependencyList& outDepList) {
    switch (typeId) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: {                                 \
            EntityDependendies deps = TYPE::GetDeps();                              \
            if (deps.hasModel)                                                      \
                outDepList.models.insert(deps.model);                               \
            for (int i = 0; i < deps.textures.size(); i++)                          \
                outDepList.textures.insert(std::string(deps.textures[i]));          \
            break;                                                                  \
        }
        EXPANDENTITIES
        #undef ENTITYEXP
    }
}

void DependencyList::GetFromLevelJson(nlohmann::json& levelData, DependencyList& outDepList) {
    for (int i = 0; i < Level::MAX_PHASES; i++) {
        auto& entitiesData = levelData["phases"][i];
        for (auto& entityData : entitiesData) {
            Entity::TypeID typeId;
            if (!entityData.contains("type_id")) {
                #define ENTITYEXP(TYPE, VAR, ID) if (entityData["name"] == TYPE::GetName()) typeId = ID;
                EXPANDENTITIES
                #undef ENTITYEXP
            }
            else
                typeId = entityData["type_id"];

            GetFromEntity(typeId, outDepList);
        }
    }
}
