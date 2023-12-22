#pragma once
#include "Entity.h"
#include "EntityID.h"
#include "EntityLimits.h"
#include "EntitySpawner.h"
#include "DestroyList.h"
#include "Types/Transform.h"
#include "Components/Components.h"
#include "Components/ComponentList.h"
#include "Resource/ResourceManager.h"
#include "Particle/ParticleManager.h"
#include "Helpers/Assert.h"
#include <deque>
#include <nlohmann/json.hpp>
#include <string>

class SpreadManager;

class EntityManager {
public:
    std::array<Entity, MAX_ENTITIES> entities_;
    std::deque<EntityID> usableEntities_;

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
    EntitySpawner spawner_;
    DestroyList destroyList_;

    void SpawnEntities();
    void DestroyEntities();
    void ClearEntitySpawnFlags();
    void Reset();

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
