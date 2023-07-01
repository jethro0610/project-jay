#include "IntervalSpawnSystem.h"
#include "ProjectileSystem.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityKey.h"
#include "../Entity/EntityManager.h"
#include "../Components/IntervalSpawnComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
using namespace glm;

constexpr EntityKey key = GetEntityKey<IntervalSpawnComponent>();
    
void IntervalSpawnSystem::Execute(
    Entity* entities,
    EntityManager& entityManager,
    IntervalSpawnComponent& intervalSpawnComponent,
    ProjectileComponent& projectileComponent,
    TransformComponent& transformComponent,
    VelocityComponent& velocityComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;
        if (!entity.MatchesKey(key))
            continue;

        int& spawnTimer = intervalSpawnComponent.spawnTimer[i];
        spawnTimer += 1;
        if (spawnTimer >= intervalSpawnComponent.spawnInterval[i]) {
            Transform spawnTransform;
            spawnTransform.position = transformComponent.transform[i].position;
            EntityID entityId = entityManager.CreateEntity(intervalSpawnComponent.entityToSpawn[i], spawnTransform);
            ProjectileSystem::Launch(entities, projectileComponent, transformComponent, velocityComponent, entityId);

            // Create the spawn entity
            spawnTimer = 0;
        }
    }
}
