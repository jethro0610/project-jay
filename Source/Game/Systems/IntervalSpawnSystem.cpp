#include "IntervalSpawnSystem.h"
#include "../Components/StaticModelComponent.h"
#include "../Systems/ProjectileSystem.h"
using namespace glm;
    
void IntervalSpawnSystem::Execute (
    EntityManager& entityManager,
    TransformComponent& transformComponent,
    IntervalSpawnComponent& intervalSpawnComponent,
    ProjectileComponent& projectileComponent,
    VelocityComponent& velocityComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entityManager.entities_[i];
        if (!entity.alive_)
            continue;
        if (!entity.HasComponent(intervalSpawnComponent))
            continue;

        uint16_t& spawnTimer = intervalSpawnComponent.spawnTimer[i];
        spawnTimer += 1;
        if (spawnTimer >= intervalSpawnComponent.spawnInterval[i]) {
            auto [entityId, transform] = entityManager.CreateEntity(intervalSpawnComponent.entityToSpawn[i]);
            transform.position_ = transformComponent.transform[i].position_;
            ProjectileSystem::Launch(projectileComponent, velocityComponent, transformComponent, entityId);

            // Create the spawn entity
            spawnTimer = 0;
        }
    }
}
