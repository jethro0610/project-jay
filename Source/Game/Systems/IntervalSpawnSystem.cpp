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
constexpr EntityKey projectileKey = GetEntityKey<ProjectileComponent>();
    
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
        if (!entity.ShouldUpdate())
            continue;
        if (!entity.MatchesKey(key))
            continue;

        int& timer = intervalSpawnComponent.timer[i];
        timer++;
        if (timer >= intervalSpawnComponent.interval[i]) {
            Transform spawnTransform;

            vec3 position = intervalSpawnComponent.offsets[i][rand() % intervalSpawnComponent.numOffsets[i]];
            
            // NOTE: Need to use the transform to get the child transform, instead of just adding position
            spawnTransform.position = transformComponent.transform[i].position + position;

            EntityID entityId = entityManager.CreateEntity(intervalSpawnComponent.entityToSpawn[i], spawnTransform);
            if (intervalSpawnComponent.launch[i])
                ProjectileSystem::Launch(
                    projectileComponent, 
                    transformComponent, 
                    velocityComponent, 
                    entityId, 
                    NULL_ENTITY
                );


            timer = 0;
        }
    }
}
