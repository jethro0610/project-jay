#pragma once
#include <deque>
#include <string>
#include <tuple>
#include "Entity.h"
#include "../Components/ComponentInclude.h"
class ResourceManager;

class EntityManager {
public:
    Entity entities_[MAX_ENTITIES];
    std::deque<EntityID> usableEntities_;

    EntityManager (ResourceManager& resourceManager);
    EntityReturn CreateEntity();
    EntityReturn CreateEntity(std::string entityName);
    void DestroyEntity(EntityID entityToDestroy);

    template <class T>
    void RegisterComponent(EntityID targetEntity) {
        assert(entities_[targetEntity].alive_);
        // assert(entities_[targetEntity].HasComponent<T>());
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
    ResourceManager& resourceManager_;
    std::tuple<
        #define COMPONENTVAR(TYPE, VAR) TYPE,
            CREATECOMPONENTVARS  
        #undef COMPONENTVAR
        uint8_t 
    > components_;

    std::unordered_map<std::string, Component*> componentMap_;
    std::unordered_map<std::string, uint8_t> componentIds_;
};
