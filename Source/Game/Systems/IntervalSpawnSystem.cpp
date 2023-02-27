#include "IntervalSpawnSystem.h"
    
void IntervalSpawnSystem::Execute (
    EntityManager& entityManager,
    IntervalSpawnComponent& intervalSpawnComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entityManager.entities_[i];
        if (!entity.alive_)
            continue;
        if (!entity.HasComponent<IntervalSpawnComponent>())
            continue;

        uint16_t& spawnTimer = intervalSpawnComponent.spawnTimer[i];
        if (spawnTimer >= intervalSpawnComponent.spawnInterval[i]) {
            uint16_t newEntity = entityManager.CreateEntity();
            // Create the spawn entity
            spawnTimer = 0;
        }
    }
}
