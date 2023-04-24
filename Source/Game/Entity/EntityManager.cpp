#include "EntityManager.h"
#include "../../Resource/ResourceManager.h"
#include "../../Logging/Logger.h"

EntityManager::EntityManager (ResourceManager& resourceManager):
    resourceManager_(resourceManager)
{
    usableEntities_.push_front(0);
    uint8_t idCounter = 0;
    
    #define COMPONENTVAR(TYPE, VAR) \
        componentMap_[TYPE::GetName()] = &std::get<TYPE>(components_); \
        componentIds_[TYPE::GetName()] = TYPE::GetID();
        CREATECOMPONENTVARS  
    #undef COMPONENTVAR
}

EntityReturn EntityManager::CreateEntity() {
    EntityID createdEntity = usableEntities_.front();
    usableEntities_.pop_front();
    if (usableEntities_.size() <= 0)
        usableEntities_.push_front(createdEntity + 1);

    entities_[createdEntity].alive_ = true;
    return { createdEntity, GetComponent<TransformComponent>().transform[createdEntity] };
}

EntityReturn EntityManager::CreateEntity(std::string entityName) {
    auto [entityId, entityTransform] = CreateEntity();
    nlohmann::json entityData = resourceManager_.entities_[entityName];
    for (auto& componentData : entityData["components"].items()) {
        std::string name = componentData.key();
        Component* component = componentMap_[name];
        uint8_t componentId = componentIds_[name];
        entities_[entityId].AddComponentById(componentId);
        component->Load(componentData.value(), entityId);
    }
    return { entityId, entityTransform };
}

void EntityManager::DestroyEntity(EntityID entityToDestroy) {
    entities_[entityToDestroy].alive_ = false;
    entities_[entityToDestroy].key_ = 0;
    usableEntities_.push_front(entityToDestroy);
}
