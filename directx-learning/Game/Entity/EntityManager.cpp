#include "EntityManager.h"

int EntityManager::CreateEntity() {
    entities_[PLAYER_ENTITY].alive_ = true;
    return 0;
}
