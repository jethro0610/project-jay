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
        if (!entity.ShouldUpdate())
            continue;
        if (!entity.MatchesKey(key))
            continue;
    }
}

typedef void (LaunchFunction)(
    float param1,
    float param2,
    vec3& projectileVelocity,
    vec3& senderVelocity,
    vec3 projectilePosition,
    vec3 senderPosition
);  

void LaunchRandom (
    float speed,
    float height,
    vec3& projectileVelocity,
    vec3& senderVelocity,
    vec3 projectilePosition,
    vec3 senderPosition
) {

}

void LaunchRiseAndFall(
    float speed,
    float height,
    vec3& projectileVelocity,
    vec3& senderVelocity,
    vec3 projectilePosition,
    vec3 senderPosition
) {

}

void LaunchTarget(
    float speed,
    float height,
    vec3& projectileVelocity,
    vec3& senderVelocity,
    vec3 projectilePosition,
    vec3 senderPosition
) {
    vec3 planarSenderVel = vec3(senderVelocity.x, 0.0f, senderVelocity.z);
    vec3 senderDirection = normalize(planarSenderVel);
    float senderMagnitude = length(planarSenderVel);

    vec3 direction = normalize(projectilePosition - senderPosition);
    direction.y = 0.0f;
    direction = normalize(senderDirection + direction * 0.8f);

    projectileVelocity = direction * senderMagnitude * 0.95f;
    projectileVelocity.y += height + senderVelocity.y * 0.5f;
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
    EntityID sender 
) {
    const float& param1 = projectileComponent.param1[projectile];
    const float& param2 = projectileComponent.param2[projectile];
    vec3& projectileVelocity = velocityComponent.velocity[projectile];
    vec3& senderVelocity = velocityComponent.velocity[sender];
    vec3 projectilePosition = transformComponent.transform[projectile].position;
    vec3 senderPosition = transformComponent.transform[sender].position;

    ASSERT((projectileComponent.type[projectile] != NumOfProjectileTypes), "Attempt to launch non-projectile entity");
    launchFunctions[projectileComponent.type[projectile]](
        param1, 
        param2, 
        projectileVelocity, 
        senderVelocity,
        projectilePosition,
        senderPosition
    );
}
