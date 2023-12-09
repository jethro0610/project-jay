#include "SpawnSystem.h"

void SpawnSystem::ClearSpawns(
    EntityList& entities
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        Entity& entity = entities[i];
        if (!entity.alive_)
            continue;

        entity.spawnedThisTick_ = false;
    }
}
