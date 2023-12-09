#include "IntervalSpawnSystem.h"
#include "Components/IntervalSpawnComponent.h"
#include "Components/ProjectileComponent.h"
#include "Components/TransformComponent.h"
#include "Components/VelocityComponent.h"
#include "Seed/SeedManager.h"
#include "Helpers/Random.h"
using namespace glm;

constexpr EntityKey key = GetEntityKey<IntervalSpawnComponent>();
    
void IntervalSpawnSystem::Execute(
    EntityList& entities,
    ComponentList& components,
    SpawnList& spawnList,
    SeedManager& seedManager
) {
    auto& intervalSpawnComponent = components.Get<IntervalSpawnComponent>();
    auto& projectileComponent = components.Get<ProjectileComponent>();
    auto& transformComponent = components.Get<TransformComponent>();
    auto& velocityComponent = components.Get<VelocityComponent>();

    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entities[i].ShouldUpdate(key)) continue;

        int& timer = intervalSpawnComponent.timer[i];
        timer++;

        if (timer >= intervalSpawnComponent.interval[i]) {
            Transform spawnTransform;
            vec3 offset = intervalSpawnComponent.offsets[i][rand() % intervalSpawnComponent.offsets[i].size()];
            vec3 radialOffset = intervalSpawnComponent.planar[i] ? 
                RandomVectorPlanar(intervalSpawnComponent.radius[i]) :
                RandomVector(intervalSpawnComponent.radius[i]);
            spawnTransform.position = transformComponent.transform[i].position + offset + radialOffset;
            // NOTE: Need to use the transform to get the child transform, instead of just adding position

            // if (intervalSpawnComponent.entityToSpawn[i] == nullptr)
            //     seedManager.CreateSeed(spawnTransform.position);
            // else
            spawnList.push_back({intervalSpawnComponent.entityToSpawn[i], spawnTransform});

            timer = 0;
        }
    }
}
