#pragma once
#include <deque>
#include <nlohmann/json.hpp>
#include <string>
#include <tuple>
#include "../../Types/Transform.h"
#include "../../Helpers/Assert.h"
#include "../../Logging/Logger.h"
#include "Entity.h"
#include "EntityID.h"
#include "EntityKey.h"
#include "EntityLimits.h"
#include "SpawnList.h"
#include "DestroyList.h"

#include "../Components/Components.h"
#include "../Components/ComponentList.h"

#include "../ResourceManager.h"
#include "../ParticleManager.h"

class SpreadManager;

class EntityManager {
public:
    std::array<Entity, MAX_ENTITIES> entities_;
    std::deque<EntityID> usableEntities_;
    std::unordered_map<std::string, nlohmann::json> entityData_;

    EntityManager(ResourceManager& resourceManager, ParticleManager& particleManager);
    EntityID CreateEntity(const Transform& transform = Transform());
    EntityID CreateEntity(EntityDescription* descripition, const Transform& transform = Transform());
    void DestroyEntity(EntityID entityToDestroy);

    template <class T>
    void RegisterComponent(EntityID targetEntity) {
        ASSERT(entities_[targetEntity].alive_, "Registered component to dead entity");
        entities_[targetEntity].AddComponent<T>();
    }

    ComponentList components_;
    SpawnList spawnList_;
    DestroyList destroyList_;

    void SpawnEntities();
    void DestroyEntities();

private:
    #define COMPONENTEXPANSION(TYPE, VAR) TYPE VAR;
    #define TAILEXPANSION(TYPE, VAR) TYPE VAR;
    EXPANDCOMPONENTS
    #undef COMPONENTEXPANSION 
    #undef TAILEXPANSION 

    std::unordered_map<std::string, Component*> componentMap_;
    std::unordered_map<std::string, int> componentIds_;
    ResourceManager& resourceManager_;
    ParticleManager& particleManager_;
};
