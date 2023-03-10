#pragma once
#include "Entity.h"
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
    std::deque<uint16_t> usableEntities_;

    EntityManager (ResourceManager& resourceManager);
    uint16_t CreateEntity();
    uint16_t CreateEntity(std::string entityName);
    void DestroyEntity(uint16_t entityToDestroy);

    void RegisterComponent(Component& component, uint16_t targetEntity) {
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
