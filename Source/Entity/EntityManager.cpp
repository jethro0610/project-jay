#include "EntityManager.h"
#include "Helpers/MapCheck.h"

EntityManager::EntityManager(ResourceManager& resourceManager, ParticleManager& particleManager) : 
components_(
    #define COMPONENTEXPANSION(TYPE, VAR) VAR,
    #define TAILEXPANSION(TYPE, VAR) VAR
    EXPANDCOMPONENTS
    #undef COMPONENTEXPANSION 
    #undef TAILEXPANSION 
),
resourceManager_(resourceManager),
particleManager_(particleManager)
{
    usableEntities_.push_front(0);

    #define COMPONENTEXPANSION(TYPE, VAR) \
    componentMap_[TYPE::GetName()] = &VAR; \
    componentIds_[TYPE::GetName()] = TYPE::GetID();
    #define TAILEXPANSION(TYPE, VAR) \
    componentMap_[TYPE::GetName()] = &VAR; \
    componentIds_[TYPE::GetName()] = TYPE::GetID();
    EXPANDCOMPONENTS
    #undef COMPONENTEXPANSION 
    #undef TAILEXPANSION 
}

EntityID EntityManager::CreateEntity(const Transform& transform) {
    EntityID createdEntity = usableEntities_.front();
    usableEntities_.pop_front();
    if (usableEntities_.size() <= 0)
        usableEntities_.push_front(createdEntity + 1);

    entities_[createdEntity].alive_ = true;
    entities_[createdEntity].spawnedThisTick_ = true;
    components_.Get<TransformComponent>().transform[createdEntity] = transform;
    components_.Get<TransformComponent>().transformLastUpdate[createdEntity] = transform;
    components_.Get<TransformComponent>().renderTransform[createdEntity] = transform;
    return createdEntity;
}

EntityID EntityManager::CreateEntity(EntityDescription* description, const Transform& transform) {
    EntityID createdEntity = CreateEntity(transform);
    ASSIGN_DEBUG_NAME(entities_[createdEntity], description->DBG_name);

    for (auto& componentData : description->data["components"].items()) {
        std::string componentName = componentData.key();
        Component* component = GetFromMap<Component*>(
            componentMap_, 
            componentName, 
            "Tried assigning non-existent component \"" + componentName + "\""
        );
        int componentId = componentIds_[componentName];
        entities_[createdEntity].AddComponentById(componentId);
        component->Load(componentData.value(), createdEntity);
    }
    entities_[createdEntity].seedsOnDestroy_ = GetInt(description->data, "seeds_on_destroy", 0);
    entities_[createdEntity].seedsRadius_ = GetInt(description->data, "seed_radius", 0);

    if (description->data.contains("emitters")) {
        const auto& emittersData = description->data["emitters"];
        ASSERT((emittersData.size() <= MAX_ENTITY_EMITTERS), "Too many emitters on entity");
        for (int i = 0; i < emittersData.size(); i++) {
            EmitterProperties* properties = resourceManager_.GetEmitterProperties(emittersData[i].get<std::string>());
            ParticleEmitter* emitter = particleManager_.RequestEmitter(properties);
            emitter->transform_ = components_.Get<TransformComponent>().renderTransform[createdEntity];
            emitter->lastTransform_ = emitter->transform_;
            entities_[createdEntity].emitters_.push_back(emitter);
        }
    }

    #ifdef _DEBUG
    if (description->data.contains("debug_collider")) {
        nlohmann::json& colliderData = description->data["debug_collider"];
        entities_[createdEntity].DBG_collider.radius = GetFloat(colliderData, "radius", 0.0f);
        entities_[createdEntity].DBG_collider.top = GetFloat(colliderData, "top", 0.0f);
        entities_[createdEntity].DBG_collider.bottom = GetFloat(colliderData, "bottom", 0.0f);
    }
    else if (description->data["components"].contains("pushbox")) {
        nlohmann::json& colliderData = description->data["components"]["pushbox"];
        entities_[createdEntity].DBG_collider.radius = GetFloat(colliderData, "radius", 0.0f);
        entities_[createdEntity].DBG_collider.top = GetFloat(colliderData, "top", 0.0f);
        entities_[createdEntity].DBG_collider.bottom = GetFloat(colliderData, "bottom", 0.0f);
    }
    else {
        entities_[createdEntity].DBG_collider.radius = 1.0f;
        entities_[createdEntity].DBG_collider.top = 1.0f;
        entities_[createdEntity].DBG_collider.bottom = 1.0f;
    }
    #endif

    return createdEntity;
}

void EntityManager::DestroyEntity(EntityID entityToDestroy) {
    Entity& entity = entities_[entityToDestroy];
    entity.alive_ = false;
    entity.key_ = 0;
    for (ParticleEmitter* emitter : entity.emitters_)
        emitter->release_ = true;
    entity.emitters_.clear();
    usableEntities_.push_front(entityToDestroy);
}

void EntityManager::SpawnEntities() {
    for (const Spawn& spawn : spawnList_)
        CreateEntity(spawn.description, spawn.transform);

    spawnList_.clear();
}

void EntityManager::DestroyEntities() {
    for (const DestroyInfo& destroyInfo : destroyList_)
        DestroyEntity(destroyInfo.entityId);

    destroyList_.clear();
}

void EntityManager::Reset() {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entities_[i].alive_)
            continue;

        destroyList_.push_back({i, false});
    }
    DestroyEntities();
    usableEntities_.clear();
    usableEntities_.push_back(0);
}
