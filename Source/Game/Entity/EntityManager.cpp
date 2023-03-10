#include "EntityManager.h"

EntityManager::EntityManager (ResourceManager& resourceManager):
    resourceManager_(resourceManager)
{
    usableEntities_.push_front(0);
    uint8_t idCounter = 0;
    
    #define COMPONENTVAR(TYPE, VAR) \
        VAR.id = idCounter++; \
        componentMap_[VAR.GetName()] = &VAR;   
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
    for (auto& componentData : entityData["components"].items()) {
        std::string name = componentData.key();
        Component* component = componentMap_[name];
        entities_[createdEntity].componentMask_.set(component->id);
        component->Load(componentData.value(), createdEntity);
    }
    return createdEntity;
}

void EntityManager::DestroyEntity(uint16_t entityToDestroy) {
    entities_[entityToDestroy].alive_ = false;
    entities_[entityToDestroy].componentMask_ = 0;
    usableEntities_.push_front(entityToDestroy);
}
