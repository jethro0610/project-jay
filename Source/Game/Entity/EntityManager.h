#pragma once
#include <deque>
#include <json.hpp>
#include <string>
#include <tuple>
#include "Entity.h"
#include "../Components/ComponentInclude.h"

class EntityManager {
public:
    Entity entities_[MAX_ENTITIES];
    std::deque<EntityID> usableEntities_;
    std::unordered_map<std::string, nlohmann::json> entityData_;

    EntityManager();
    void LoadEntity(std::string entityName);
    EntityReturn CreateEntity();
    EntityReturn CreateEntity(std::string entityName);
    void DestroyEntity(EntityID entityToDestroy);

    template <class T>
    void RegisterComponent(EntityID targetEntity) {
        assert(entities_[targetEntity].alive_);
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
        uint8_t 
    > components_;

    std::unordered_map<std::string, Component*> componentMap_;
    std::unordered_map<std::string, uint8_t> componentIds_;
};
