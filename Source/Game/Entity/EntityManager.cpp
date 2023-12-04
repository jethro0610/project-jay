#include "EntityManager.h"
#include "../../Helpers/MapCheck.h"
#include "../../Logging/Logger.h"

EntityManager::EntityManager(ParticleManager& particleManager) : 
components_(
    #define COMPONENTEXPANSION(TYPE, VAR) VAR,
    #define TAILEXPANSION(TYPE, VAR) VAR
    EXPANDCOMPONENTS
    #undef COMPONENTEXPANSION 
    #undef TAILEXPANSION 
),
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

void EntityManager::LoadEntity(const std::string& name) {
    ForceMapUnique(entityData_, name, "Entity " + name + " is already loaded");

    std::ifstream inFile("entities/" + name + ".json");
    ASSERT(inFile.is_open(), "Failed to load entity " + name);

    nlohmann::json data = nlohmann::json::parse(inFile);
    entityData_[name] = data;
    DEBUGLOG("Loaded entity " << name);
}

EntityID EntityManager::CreateEntity(const Transform& transform) {
    EntityID createdEntity = usableEntities_.front();
    usableEntities_.pop_front();
    if (usableEntities_.size() <= 0)
        usableEntities_.push_front(createdEntity + 1);

    entities_[createdEntity].alive_ = true;
    components_.Get<TransformComponent>().transform[createdEntity] = transform;
    components_.Get<TransformComponent>().transformLastUpdate[createdEntity] = transform;
    components_.Get<TransformComponent>().renderTransform[createdEntity] = transform;
    return createdEntity;
}

EntityID EntityManager::CreateEntity(const std::string& name, const Transform& transform) {
    EntityID createdEntity = CreateEntity(transform);
    nlohmann::json& entityData = GetFromMap<nlohmann::json>(entityData_, name, "Tried creating unloaded entity " + name);

    for (auto& componentData : entityData["components"].items()) {
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
    entities_[createdEntity].seedsOnDestroy_ = GetInt(entityData, "seeds_on_destroy", 0);
    entities_[createdEntity].seedsRadius_ = GetInt(entityData, "seed_radius", 0);

    if (entityData.contains("emitters")) {
        const auto& emittersData = entityData["emitters"];
        ASSERT((emittersData.size() <= MAX_ENTITY_EMITTERS), "Too many emitters on entity");
        for (int i = 0; i < emittersData.size(); i++) {
            ParticleEmitter* emitter = particleManager_.RequestEmitter(emittersData[i].get<std::string>());
            emitter->parent_ = &components_.Get<TransformComponent>().renderTransform[createdEntity];
            emitter->lastTransform_ = components_.Get<TransformComponent>().renderTransform[createdEntity];
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
        CreateEntity(spawn.name, spawn.transform);

    spawnList_.clear();
}

void EntityManager::DestroyEntities() {
    for (const DestroyInfo& destroyInfo : destroyList_)
        DestroyEntity(destroyInfo.entityId);

    destroyList_.clear();
}
