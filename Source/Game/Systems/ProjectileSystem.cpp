#include "ProjectileSystem.h"
#include "../Components/VelocityComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../../Helpers/Random.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityKey.h"
using namespace glm;

constexpr EntityKey projectileKey = GetEntityKey<ProjectileComponent, VelocityComponent>();

void Launch(
    ComponentList& components,
    EntityID projectile
) {
    ProjectileComponent& projectileComponent = components.Get<ProjectileComponent>(); 
    VelocityComponent& velocityComponent = components.Get<VelocityComponent>(); 

    velocityComponent.velocity[projectile] = RandomVector(
        projectileComponent.minLaunchVelocity[projectile],
        projectileComponent.maxLaunchVelocity[projectile]
    );
}

void ProjectileSystem::Execute(
    EntityList& entitites,
    ComponentList& components
) {
    ProjectileComponent& projectileComponent = components.Get<ProjectileComponent>(); 
    VelocityComponent& velocityComponent = components.Get<VelocityComponent>(); 

    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entitites[i];
        if (!entity.ShouldUpdate(projectileKey)) continue;

        if (entity.spawnedThisTick_ && projectileComponent.launchOnSpawn[i])
            Launch(components, i);
    }
}
    
