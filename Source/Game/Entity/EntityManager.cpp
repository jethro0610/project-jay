#include "EntityManager.h"

EntityManager::EntityManager (ResourceManager& resourceManager):
    resourceManager_(resourceManager)
{
    usableEntities_.push_front(0);
    
    #define COMPONENTVAR(TYPE, VAR) \
        componentIdMap_[VAR.GetID()] = &VAR;   
        CREATECOMPONENTVARS  
    #undef COMPONENTVAR
}

uint16_t EntityManager::CreateEntity() {
    uint16_t createdEntity = usableEntities_.front();
    usableEntities_.pop_front();
    if (usableEntities_.size() <= 0)
        usableEntities_.push_front(createdEntity + 1);

    entities_[createdEntity].alive_ = true;
    return createdEntity;
}

uint16_t EntityManager::CreateEntity(std::string entityName) {
    uint16_t createdEntity = CreateEntity();
    nlohmann::json entityData = resourceManager_.entities_[entityName];
    for (auto& componentData : entityData["components"]) {
        uint8_t id = componentData["id"].get<double>();
        entities_[createdEntity].componentMask_.set(id);
        componentIdMap_[id]->Load(componentData, createdEntity);
    }
    return createdEntity;
}

void EntityManager::DestroyEntity(uint16_t entityToDestroy) {
    entities_[entityToDestroy].alive_ = false;
    entities_[entityToDestroy].componentMask_ = 0;
    usableEntities_.push_front(entityToDestroy);
}
