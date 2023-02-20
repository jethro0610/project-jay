#include "EntityManager.h"

uint16_t EntityManager::CreateEntity() {
    entities_[entityCount_].alive_ = true;
    return entityCount_++;
}

void EntityManager::DestroyEntity(uint16_t entityToDestroy) {
    entities_[entityToDestroy].alive_ = false;
    entityCount_--;
}
