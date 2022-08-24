#include "EntityManager.h"

int EntityManager::CreateEntity() {
    entities_[PLAYER_ENTITY].alive = true;
    return 0;
}

/*
void EntityManager::RegisterComponent(int targetEntity, int componentId) {
    assert(entities_[PLAYER_ENTITY].alive);
    assert(!entities_[PLAYER_ENTITY].componentMask.test(componentId));

    entities_[PLAYER_ENTITY].componentMask.set(componentId);
}
*/