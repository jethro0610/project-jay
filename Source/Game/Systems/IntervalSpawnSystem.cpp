#include "IntervalSpawnSystem.h"
#include "ProjectileSystem.h"
#include "../Entity/Entity.h"
#include "../../Helpers/EntityHelpers.h"
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

        uint16_t& spawnTimer = intervalSpawnComponent.spawnTimer[i];
        spawnTimer += 1;
        if (spawnTimer >= intervalSpawnComponent.spawnInterval[i]) {
            auto [entityId, transform] = entityManager.CreateEntity(intervalSpawnComponent.entityToSpawn[i]);
            transform.position_ = transformComponent.transform[i].position_;
            ProjectileSystem::Launch(entities, projectileComponent, transformComponent, velocityComponent, entityId);

            // Create the spawn entity
            spawnTimer = 0;
        }
    }
}
