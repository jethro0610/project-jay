#include "ProjectileSystem.h"
#include <glm/gtx/compatibility.hpp>
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

typedef void (LaunchFunction)(
    ProjParam1 param1,
    ProjParam2 param2,
    vec3& projectileVelocity,
    vec3& senderVelocity,
    vec3 projectilePosition,
    vec3 senderPosition
);  
void LaunchRandom (
    ProjParam1 param1,
    ProjParam2 param2,
    vec3& projectileVelocity,
    vec3& senderVelocity,
    vec3 projectilePosition,
    vec3 senderPosition
) {

}
void LaunchRiseAndFall(
    ProjParam1 param1,
    ProjParam2 param2,
    vec3& projectileVelocity,
    vec3& senderVelocity,
    vec3 projectilePosition,
    vec3 senderPosition
) {

}
void LaunchTarget(
    ProjParam1 param1,
    ProjParam2 param2,
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
    projectileVelocity.y += param2.height + senderVelocity.y * 0.5f;
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
    const ProjParam1& param1 = projectileComponent.param1[projectile];
    const ProjParam2& param2 = projectileComponent.param2[projectile];
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
    projectileComponent.active[projectile] = true;
}

typedef void (UpdateFunction)(
    ProjParam1 param1,
    ProjParam2 param2,
    vec3& projectilePosition,
    vec3& projectileVelocity,
    bool hasTarget,
    vec3 targetPosition
);  
void UpdateDefault(
    ProjParam1 param1,
    ProjParam2 param2,
    vec3& projectilePosition,
    vec3& projectileVelocity,
    bool hasTarget,
    vec3 targetPosition
) {

}
void UpdateTarget(
    ProjParam1 param1,
    ProjParam2 param2,
    vec3& projectilePosition,
    vec3& projectileVelocity,
    bool hasTarget,
    vec3 targetPosition
) {
    if (!hasTarget)
        return;

    vec3 direction = normalize(targetPosition - projectilePosition);
    projectileVelocity = lerp(projectileVelocity, direction * 150.0f, param1.trackingStrength);
}

UpdateFunction* updateFunctions[NumOfProjectileTypes] = {
    &UpdateDefault,
    &UpdateDefault,
    &UpdateTarget
};

constexpr EntityKey key = GetEntityKey<ProjectileComponent, TransformComponent, VelocityComponent>();
constexpr EntityKey targetKey = GetEntityKey<TransformComponent>();
void ProjectileSystem::Execute(
    Entity* entities,
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
        if (!projectileComponent.active[i])
            continue;

        vec3& position = transformComponent.transform[i].position; 
        vec3& velocity = velocityComponent.velocity[i]; 
        ProjParam1 param1 = projectileComponent.param1[i];
        ProjParam2 param2 = projectileComponent.param2[i];

        if (projectileComponent.target[i] == NULL_ENTITY) {
            for (int j = 0; j < MAX_ENTITIES; j++) {
                const Entity& target = entities[j];
                if (!target.alive_)
                    continue;
                if (!target.MatchesKey(targetKey))
                    continue;
                if (!transformComponent.targetable[j])
                    continue;

                vec3 targetPosition = transformComponent.transform[j].position;
                float dist = distance(vec2(position.x, position.z), vec2(targetPosition.x, targetPosition.z));
                float maxDist = (position.y - targetPosition.y) * 5.0f;
                // TODO: Use closest target only
                if (dist < maxDist) {
                    projectileComponent.target[i] = j;
                    break;
                }
            }
        }

        EntityID targetId = projectileComponent.target[i];
        vec3 targetPosition = vec3(0.0f);
        if (targetId != NULL_ENTITY)
            targetPosition = transformComponent.transform[targetId].position; 

        ProjectileType type = projectileComponent.type[i];
        updateFunctions[type](
            param1, 
            param2,
            position, 
            velocity, 
            (targetId != NULL_ENTITY), 
            targetPosition
        );
    }
}
