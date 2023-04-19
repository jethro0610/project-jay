#include "IntervalSpawnSystem.h"
#include "../Entity/EntityManager.h"
#include "ProjectileSystem.h"
using namespace glm;
    
void IntervalSpawnSystem::Execute (EntityManager& entityManager) {
    TransformComponent& transformComponent = entityManager.transformComponent_;
    IntervalSpawnComponent& intervalSpawnComponent = entityManager.intervalSpawnComponent_;
    ProjectileComponent& projectileComponent = entityManager.projectileComponent_;
    VelocityComponent& velocityComponent = entityManager.velocityComponent_;
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
            ProjectileSystem::Launch(entityManager, entityId);

            // Create the spawn entity
            spawnTimer = 0;
        }
    }
}
