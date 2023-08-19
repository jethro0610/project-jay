#include "ProjectileSystem.h"
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/string_cast.hpp>
#include "../Entity/Entity.h"
#include "../Entity/EntityKey.h"
#include "../../Constants/GameConstants.h"
#include "../World/World.h"
#include "../Components/MeterComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../../Logging/Logger.h"
#include "../../Helpers/Assert.h"
using namespace glm;

typedef void (LaunchFunction)(
    ProjParam1 param1,
    ProjParam2 param2,
    vec3& velocity,
    quat& angularVelocity,
    vec3& senderVelocity,
    vec3 projectilePosition,
    vec3 senderPosition
);  
void LaunchRandom (
    ProjParam1 param1,
    ProjParam2 param2,
    vec3& velocity,
    quat& angularVelocity,
    vec3& senderVelocity,
    vec3 projectilePosition,
    vec3 senderPosition
) {

}
void LaunchRiseAndFall(
    ProjParam1 param1,
    ProjParam2 param2,
    vec3& velocity,
    quat& angularVelocity,
    vec3& senderVelocity,
    vec3 projectilePosition,
    vec3 senderPosition
) {

}
void LaunchTarget(
    ProjParam1 param1,
    ProjParam2 param2,
    vec3& velocity,
    quat& angularVelocity,
    vec3& senderVelocity,
    vec3 projectilePosition,
    vec3 senderPosition
) {
    vec3 planarSenderVel = vec3(senderVelocity.x, 0.0f, senderVelocity.z);
    vec3 senderDirection = normalize(planarSenderVel);
    float senderMagnitude = length(planarSenderVel);

    vec3 direction = normalize(projectilePosition - senderPosition);
    direction.y = 0.0f;

    vec3 rotationDirection = normalize(direction);
    direction = normalize(senderDirection + direction * 0.8f);

    angularVelocity = quat(cross(Transform::worldUp, rotationDirection * senderMagnitude * 0.001f));

    velocity = direction * senderMagnitude * 0.95f;
    velocity.y += param2.height + senderVelocity.y * 0.5f;
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
    vec3& velocity = velocityComponent.velocity[projectile];
    quat& angularVelocity = velocityComponent.angularVelocity[projectile];
    vec3& senderVelocity = velocityComponent.velocity[sender];
    vec3 projectilePosition = transformComponent.transform[projectile].position;
    vec3 senderPosition = transformComponent.transform[sender].position;

    ASSERT((projectileComponent.type[projectile] != NumOfProjectileTypes), "Attempt to launch non-projectile entity");
    launchFunctions[projectileComponent.type[projectile]](
        param1, 
        param2, 
        velocity, 
        angularVelocity,
        senderVelocity,
        projectilePosition,
        senderPosition
    );
    projectileComponent.active[projectile] = true;
}

typedef void (ContactFunction)(
    ProjParam1 param1,
    ProjParam2 param2,
    int damage,
    vec3& projectilePosition,
    vec3& projectileVelocity,
    bool& active,
    int& meter
);  
void ContactSelfDamage(
    ProjParam1 param1,
    ProjParam2 param2,
    int damage,
    vec3& projectilePosition,
    vec3& projectileVelocity,
    bool& active,
    int& meter
) {
    meter -= damage;
}
void ContactBounceBack(
    ProjParam1 param1,
    ProjParam2 param2,
    int damage,
    vec3& projectilePosition,
    vec3& projectileVelocity,
    bool& active,
    int& meter
) {
    vec3 direction = vec3(projectileVelocity.x, 0.0f, projectileVelocity.z);
    projectileVelocity = normalize(direction) * 20.0f;
    projectileVelocity.y = 20.0f;
}
void ContactDeactivate(
    ProjParam1 param1,
    ProjParam2 param2,
    int damage,
    vec3& projectilePosition,
    vec3& projectileVelocity,
    bool& active,
    int& meter
) {
    active = false;
}
ContactFunction* contactFunctions[NumContactBehaviors] = {
    &ContactSelfDamage,
    &ContactBounceBack,
    &ContactDeactivate
};

void ProjectileSystem::HandleContact(
    MeterComponent& meterComponent,
    ProjectileComponent& projectileComponent, 
    TransformComponent& transformComponent, 
    VelocityComponent& velocityComponent, 
    EntityID projectile,
    EntityID reciever
) {
    vec3& position = transformComponent.transform[projectile].position; 
    vec3& velocity = velocityComponent.velocity[projectile]; 
    int damage = projectileComponent.damage[projectile];
    ProjParam1 param1 = projectileComponent.param1[projectile];
    ProjParam2 param2 = projectileComponent.param2[projectile];
    bool& active = projectileComponent.active[projectile];
    int& meter = meterComponent.meter[projectile]; 

    for (int i = 0; i < NumContactBehaviors; i++)
        contactFunctions[i](param1, param2, damage, position, velocity, active, meter);
}

typedef void (UpdateFunction)(
    ProjParam1 param1,
    ProjParam2 param2,
    Transform& transform,
    vec3& velocity,
    quat& angularVelocity,
    bool hasTarget,
    vec3 targetPosition
);  
void UpdateDefault(
    ProjParam1 param1,
    ProjParam2 param2,
    Transform& transform,
    vec3& velocity,
    quat& angularVelocity,
    bool hasTarget,
    vec3 targetPosition
) {

}
void UpdateTarget(
    ProjParam1 param1,
    ProjParam2 param2,
    Transform& transform,
    vec3& velocity,
    quat& angularVelocity,
    bool hasTarget,
    vec3 targetPosition
) {
    if (!hasTarget)
        return;

    vec3 direction = normalize(targetPosition - transform.position);
    velocity = lerp(velocity, direction * 150.0f, param1.trackingStrength);
    angularVelocity = slerp(angularVelocity, quat(1.0f, 0.0f, 0.0f, 0.0f), 0.05f);
}

UpdateFunction* updateFunctions[NumOfProjectileTypes] = {
    &UpdateDefault,
    &UpdateDefault,
    &UpdateTarget
};

constexpr EntityKey key = GetEntityKey<ProjectileComponent, TransformComponent, VelocityComponent>();
constexpr EntityKey targetKey = GetEntityKey<TransformComponent>();
void ProjectileSystem::Execute(
    std::array<Entity, MAX_ENTITIES>& entities,
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

        if (projectileComponent.target[i] != NULL_ENTITY && entities[projectileComponent.target[i]].alive_ == false)
            projectileComponent.target[i] = NULL_ENTITY;
        else {
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
            transformComponent.transform[i], 
            velocity, 
            velocityComponent.angularVelocity[i],
            (targetId != NULL_ENTITY), 
            targetPosition
        );
    }
}
