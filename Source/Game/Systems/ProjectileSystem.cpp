#include "ProjectileSystem.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../../Helpers/Random.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityKey.h"
using namespace glm;

constexpr EntityKey projectileKey = GetEntityKey<ProjectileComponent, TransformComponent, VelocityComponent>();

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
    TransformComponent& transformComponent = components.Get<TransformComponent>(); 
    VelocityComponent& velocityComponent = components.Get<VelocityComponent>(); 

    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entitites[i];
        if (!entity.ShouldUpdate(projectileKey)) continue;

        if (entity.spawnedThisTick_ && projectileComponent.launchOnSpawn[i])
            Launch(components, i);

        if (projectileComponent.emitter[i] != nullptr) {
            projectileComponent.emitter[i]->parent_ = &transformComponent.renderTransform[i];
            projectileComponent.emitter[i]->active_ = true;
        }
    }
}
    
