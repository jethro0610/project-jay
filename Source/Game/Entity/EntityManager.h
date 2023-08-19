#pragma once
#include <deque>
#include <json.hpp>
#include <string>
#include <tuple>
#include "Entity.h"
#include "EntityID.h"
#include "../../Helpers/Assert.h"
#include "../../Logging/Logger.h"
#include "../Components/ComponentInclude.h"

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

    template <class T>
    T& GetComponent() {
        return std::get<T>(components_);
    }

    template <class...T>
    std::tuple<T&...> GetComponents() {
        return std::make_tuple(std::get<T>(components_)...);
    }

private:
    std::tuple<
        #define COMPONENTVAR(TYPE, VAR) TYPE,
            CREATECOMPONENTVARS  
        #undef COMPONENTVAR
        int 
    > components_;

    std::unordered_map<std::string, Component*> componentMap_;
    std::unordered_map<std::string, int> componentIds_;
};
