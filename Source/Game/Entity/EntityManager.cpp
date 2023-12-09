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

    for (auto& componentData : (*description)["components"].items()) {
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
    entities_[createdEntity].seedsOnDestroy_ = GetInt(*description, "seeds_on_destroy", 0);
    entities_[createdEntity].seedsRadius_ = GetInt(*description, "seed_radius", 0);

    if (description->contains("emitters")) {
        const auto& emittersData = (*description)["emitters"];
        ASSERT((emittersData.size() <= MAX_ENTITY_EMITTERS), "Too many emitters on entity");
        for (int i = 0; i < emittersData.size(); i++) {
            EmitterProperties* properties = resourceManager_.GetEmitterProperties(emittersData[i].get<std::string>());
            ParticleEmitter* emitter = particleManager_.RequestEmitter(properties);
            emitter->transform_ = components_.Get<TransformComponent>().renderTransform[createdEntity];
            emitter->lastTransform_ = emitter->transform_;
            entities_[createdEntity].emitters_.push_back(emitter);
        }
    }

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
