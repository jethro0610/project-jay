#include "ProjectileSystem.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityKey.h"
#include "../../Constants/GameConstants.h"
#include "../World/World.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../../Logging/Logger.h"
#include "../../Helpers/Assert.h"
using namespace glm;

constexpr EntityKey key = GetEntityKey<ProjectileComponent, TransformComponent, VelocityComponent>();

void ProjectileSystem::CalculateVelocities(
    Entity* entities, 
    World& world,
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
    }
}

typedef void (LaunchFunction)(const float& param1, const float& param2, vec3& velocity);  

void LaunchRandom (
    const float& speed,
    const float& height,
    vec3& velocity
) {

}

void LaunchRiseAndFall(
    const float& speed,
    const float& height,
    vec3& velocity
) {

}

void LaunchTarget(
    const float& speed,
    const float& height,
    vec3& velocity
) {
   velocity.y = height; 
}

LaunchFunction* launchFunctions[NumOfProjectileTypes] = {
    &LaunchRandom,
    &LaunchRiseAndFall,
    &LaunchTarget
};

void ProjectileSystem::Launch(
    ProjectileComponent& projectileComponent, 
    TransformComponent& transformComponent, 
    VelocityComponent& velocityComponent, 
    EntityID projectile,
    EntityID target
) {
    const float& param1 = projectileComponent.param1[projectile];
    const float& param2 = projectileComponent.param2[projectile];
    vec3& velocity = velocityComponent.velocity[projectile];

    ASSERT((projectileComponent.type[projectile] != NumOfProjectileTypes), "Attempt to launch non-projectile entity");
    launchFunctions[projectileComponent.type[projectile]](param1, param2, velocity);
}
