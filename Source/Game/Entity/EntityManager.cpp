#include "EntityManager.h"
#include "../../Helpers/MapCheck.h"
#include "../../Logging/Logger.h"

EntityManager::EntityManager() : components_(
    #define COMPONENTEXPANSION(TYPE, VAR) VAR,
    #define TAILEXPANSION(TYPE, VAR) VAR
    EXPANDCOMPONENTS
    #undef COMPONENTEXPANSION 
    #undef TAILEXPANSION 
) {
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
    return createdEntity;
}

void EntityManager::DestroyEntity(EntityID entityToDestroy) {
    entities_[entityToDestroy].alive_ = false;
    entities_[entityToDestroy].key_ = 0;
    usableEntities_.push_front(entityToDestroy);
}

void EntityManager::SpawnEntities() {
    for (const Spawn& spawn : spawnList_)
        CreateEntity(spawn.name, spawn.transform);

    spawnList_.clear();
}

void EntityManager::DestoryEntities() {
    for (const EntityID& entity : destroyList_)
        DestroyEntity(entity);

    destroyList_.clear();
}
