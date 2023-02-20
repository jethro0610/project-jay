#include "EntityManager.h"

EntityManager::EntityManager() {
    usableEntities_.push_front(0);
}

uint16_t EntityManager::CreateEntity() {
    uint16_t createdEntity = usableEntities_.front();
    usableEntities_.pop_front();
    if (usableEntities_.size() <= 0)
        usableEntities_.push_front(createdEntity + 1);

    entities_[createdEntity].alive_ = true;
    return createdEntity;
}

void EntityManager::DestroyEntity(uint16_t entityToDestroy) {
    entities_[entityToDestroy].alive_ = false;
    entities_[entityToDestroy].componentMask_ = 0;
    usableEntities_.push_front(entityToDestroy);
}
