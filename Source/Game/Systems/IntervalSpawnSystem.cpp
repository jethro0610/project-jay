#include "IntervalSpawnSystem.h"
#include "ProjectileSystem.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityKey.h"
#include "../Entity/EntityManager.h"
#include "../World/SeedManager.h"
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
    SeedManager& seedManager,
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

            // Interval
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

            // Seeded
            spawnTransform.position = transformComponent.transform[i].position;

            if (intervalSpawnComponent.entityToSpawn[i] == "e_seed") {
                float x = ((rand() % 100) * 0.01f) - 0.5f;
                float z = ((rand() % 100) * 0.01f) - 0.5f;
                vec2 direction = normalize(vec2(x, z));
                float dist = (rand() % 100) * 0.01f;
                dist = sqrt(dist);
                direction *= dist * 30.0f;
                float y = ((rand() % 4000) * 0.01f) - 20.0f;

                seedManager.CreateSeed(spawnTransform.position + vec3(direction.x, y + 30.0f, direction.y));
            }
            else
                EntityID entityId = entityManager.CreateEntity(intervalSpawnComponent.entityToSpawn[i], spawnTransform);
            timer = 0;
        }
    }
}
