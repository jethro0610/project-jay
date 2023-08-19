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
#include "../../Helpers/Random.h"
using namespace glm;

constexpr EntityKey key = GetEntityKey<IntervalSpawnComponent>();
constexpr EntityKey projectileKey = GetEntityKey<ProjectileComponent>();
    
void IntervalSpawnSystem::Execute(
    std::array<Entity, MAX_ENTITIES>& entities,
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
            vec3 offset = intervalSpawnComponent.offsets[i][rand() % intervalSpawnComponent.offsets[i].size()];
            vec3 radialOffset = RandomVector(intervalSpawnComponent.radius[i]);
            spawnTransform.position = transformComponent.transform[i].position + offset + radialOffset;
            // NOTE: Need to use the transform to get the child transform, instead of just adding position

            if (intervalSpawnComponent.entityToSpawn[i] == "e_seed") {
                seedManager.CreateSeed(spawnTransform.position);
            }
            else {
                EntityID entityId = entityManager.CreateEntity(intervalSpawnComponent.entityToSpawn[i], spawnTransform);
                if (intervalSpawnComponent.launch[i])
                    ProjectileSystem::Launch(
                        projectileComponent, 
                        transformComponent, 
                        velocityComponent, 
                        entityId, 
                        NULL_ENTITY
                    );
            }

            timer = 0;
        }
    }
}
