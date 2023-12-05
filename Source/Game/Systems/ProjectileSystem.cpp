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
    EntityList& entities,
    ComponentList& components,
    EntityID projectile
) {
    const Entity& entity = entities[projectile];
    ProjectileComponent& projectileComponent = components.Get<ProjectileComponent>(); 
    TransformComponent& transformComponent = components.Get<TransformComponent>(); 
    VelocityComponent& velocityComponent = components.Get<VelocityComponent>(); 

    velocityComponent.velocity[projectile] = RandomVector(
        projectileComponent.minLaunchVelocity[projectile],
        projectileComponent.maxLaunchVelocity[projectile]
    );
    projectileComponent.launched[projectile] = true;
}

void ProjectileSystem::Execute(
    EntityList& entities,
    ComponentList& components
) {
    ProjectileComponent& projectileComponent = components.Get<ProjectileComponent>(); 
    TransformComponent& transformComponent = components.Get<TransformComponent>(); 
    VelocityComponent& velocityComponent = components.Get<VelocityComponent>(); 

    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.ShouldUpdate(projectileKey)) continue;

        if (entity.spawnedThisTick_ && projectileComponent.launchOnSpawn[i])
            Launch(entities, components, i);

        if (!projectileComponent.launched[i])
            continue;

        for (ParticleEmitter* emitter : entity.emitters_)
            emitter->active_ = true;

        vec3 normVel = normalize(velocityComponent.velocity[i]);
        transformComponent.transform[i].rotation = quatLookAtRH(
            normVel, 
            Transform::worldUp
        );
    }
}
