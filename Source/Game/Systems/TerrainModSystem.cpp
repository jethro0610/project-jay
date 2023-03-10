#include "TerrainModSystem.h"

void TerrainModSystem::Execute(EntityManager& entityManager, TerrainModComponent& terrainModComponent) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entityManager.entities_[i];
        if (!entity.alive_)
            continue;
        if (!entity.HasComponent(terrainModComponent))
            continue;

        /* terrainModComponent.radius[i] += 0.1f; */
    }
}
