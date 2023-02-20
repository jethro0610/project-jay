#include "TerrainModSystem.h"

void TerrainModSystem::Execute(Entity* entities, TerrainModComponent& terrainModComponent) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;
        if (!entity.HasComponent<TerrainModComponent>())
            continue;

        /* terrainModComponent.radius[i] += 0.1f; */
    }
}
