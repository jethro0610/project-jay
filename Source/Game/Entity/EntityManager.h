#pragma once
#include <deque>
#include <nlohmann/json.hpp>
#include <string>
#include <tuple>
#include "Entity.h"
#include "EntityID.h"
#include "EntityKey.h"
#include "../../Helpers/Assert.h"
#include "../../Logging/Logger.h"
#include "../Components/ComponentInclude.h"
#include "../Components/ComponentList.h"

class EntityManager {
public:
    std::array<Entity, MAX_ENTITIES> entities_;
    std::deque<EntityID> usableEntities_;
    std::unordered_map<std::string, nlohmann::json> entityData_;

    EntityManager();
    void LoadEntity(std::string name);
    EntityID CreateEntity(Transform transform = Transform());
    EntityID CreateEntity(std::string name, Transform transform = Transform());
    void DestroyEntity(EntityID entityToDestroy);

    template <class T>
    void RegisterComponent(EntityID targetEntity) {
        ASSERT(entities_[targetEntity].alive_, "Registered component to dead entity");
        entities_[targetEntity].AddComponent<T>();
    }

    ComponentList components_;

private:
    #define COMPONENTEXPANSION(TYPE, VAR) TYPE VAR;
    #define TAILEXPANSION(TYPE, VAR) TYPE VAR;
    EXPANDCOMPONENTS
    #undef COMPONENTEXPANSION 
    #undef TAILEXPANSION 

    std::unordered_map<std::string, Component*> componentMap_;
    std::unordered_map<std::string, int> componentIds_;
};
