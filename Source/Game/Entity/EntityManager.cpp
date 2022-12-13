#include "EntityManager.h"

uint16_t EntityManager::CreateEntity() {
    entities_[entityCount_].alive_ = true;
    return entityCount_++;
}
