#pragma once
#include "Entity.h"
#include "EntityConstants.h"
#include "../../Resource/ResourceManager.h"
#include "../Components/ComponentInclude.h"
#include <bitset>
#include <tuple>
#include <unordered_map>
#include <deque>
#include <string>

class EntityManager {
public:
    Entity entities_[MAX_ENTITIES];
    std::deque<EntityID> usableEntities_;

    EntityManager (ResourceManager& resourceManager);
    EntityReturn CreateEntity();
    EntityReturn CreateEntity(std::string entityName);
    void DestroyEntity(EntityID entityToDestroy);

    void RegisterComponent(Component& component, EntityID targetEntity) {
        assert(entities_[targetEntity].alive_);
        assert(!entities_[targetEntity].componentMask_.test(component.id));
        entities_[targetEntity].componentMask_.set(component.id);
    }

    #define COMPONENTVAR(TYPE, VAR) TYPE VAR;
        CREATECOMPONENTVARS  
    #undef COMPONENTVAR

private:
    ResourceManager& resourceManager_;
    std::unordered_map<std::string, Component*> componentMap_;
};
