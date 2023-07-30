#include "CollisionSystem.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityKey.h"
#include "../World/SeedManager.h"
#include "../World/SpreadManager.h"
#include "../Components/ColliderComponent.h"
#include "../Components/MeterComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Systems/ProjectileSystem.h"
#include "../../Constants/GameConstants.h"
#include "../../Helpers/Assert.h"
using namespace glm;

constexpr EntityKey key = GetEntityKey<ColliderComponent, TransformComponent>();

int CollisionSystem::GetCollisions(
    CollisionArgs args,
    Collision* collisions
) {
    int numCollisions = 0;
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity1 = args.entities[i];
        if (!entity1.ShouldUpdate())
            continue;
        if (!entity1.MatchesKey(key))
            continue;
        vec3 entity1Pos = args.transformComponent.transform[i].position;
        float entity1Radius = args.colliderComponent.radius0[i];
        entity1Radius *= args.transformComponent.transform[i].scale.x;

        // TODO: Only check components within the same chunk
        for (int j = i + 1; j < MAX_ENTITIES; j++) {
            if (j == i)
                continue;
            const Entity& entity2 = args.entities[j];
            if (!entity2.ShouldUpdate())
                continue;
            if (!entity2.MatchesKey(key))
                continue;

            vec3 entity2Pos = args.transformComponent.transform[j].position;    
            float entity2Radius = args.colliderComponent.radius0[j]; 
            entity2Radius *= args.transformComponent.transform[j].scale.x;
            
            float dist = distance(entity1Pos, entity2Pos);
            vec3 direction = normalize(entity2Pos - entity1Pos);
            float maxDist = entity1Radius + entity2Radius;
            if (dist < maxDist) {
                Collision collision = { i, j, direction * (maxDist - dist)};
                collisions[numCollisions++] = collision;
            }
        }
    }
    ASSERT((numCollisions < MAX_COLLISIONS), "Exceeded max collisions");
    return numCollisions;
}

typedef void (RecieveMeteorFunc)(CollisionArgs args, EntityID sender, EntityID reciever);
void RecieveMeteorLaunch(CollisionArgs args, EntityID sender, EntityID reciever) {
    ProjectileSystem::Launch(
        args.projectileComponent, 
        args.transformComponent, 
        args.velocityComponent, 
        reciever, 
        sender
    );
}

void RecieveMeteorDamage(CollisionArgs args, EntityID sender, EntityID reciever) {
    args.meterComponent.meter[reciever] -= args.colliderComponent.recieveMeteorDamage[reciever];
}
void RecieveMeteorReleaseSeed(CollisionArgs args, EntityID sender, EntityID reciever) {
    args.seedManager.CreateMultipleSeed(
        args.transformComponent.transform[reciever].position, 
        args.colliderComponent.recieveMeteorSeedAmount[reciever], 
        10
    );
}

RecieveMeteorFunc* meteorBehaviorFuncs[MAX_RECIEVE_METEOR_BEHAVIORS] = {
    &RecieveMeteorLaunch,
    &RecieveMeteorDamage,
    &RecieveMeteorReleaseSeed
};

void MeteorSlowdown(vec3& senderVeloicty, int& senderCooldown, int& consecutiveSends) {
    senderCooldown = 10;

    // NOTE: Renable this to slowdown on consecutive meteors
    return;
    consecutiveSends++;
    if (consecutiveSends < 3)
        return;

    vec3 planarSenderVelocity = vec3(senderVeloicty.x, 0.0f, senderVeloicty.z);
    planarSenderVelocity *= 0.6f;
    senderVeloicty.x = planarSenderVelocity.x;
    senderVeloicty.z = planarSenderVelocity.z;
}

constexpr EntityKey projectileKey = GetEntityKey<ProjectileComponent>();

void HandleCollision(
    CollisionArgs args,
    EntityID sender,
    EntityID reciever,
    vec3 resolutionVec
) {
    std::bitset<MAX_COLLIDER_PROPERTIES> senderProps = args.colliderComponent.properties[sender];
    std::bitset<MAX_COLLIDER_PROPERTIES> recieverProps = args.colliderComponent.properties[reciever];
    std::bitset<MAX_RECIEVE_METEOR_BEHAVIORS> recieverBehaviors = args.colliderComponent.recieveMeteorBehaviors[reciever];

    int& recieverCooldown = args.colliderComponent.recieveMeteorCooldown[reciever]; 
    int& senderCooldown = args.colliderComponent.recieveMeteorCooldown[sender]; 

    if (
        length(args.velocityComponent.velocity[sender]) > METEOR_SPEED &&
        senderProps.test(SendMeteor) && 
        recieverProps.test(RecieveMeteor) &&
        recieverCooldown == 0
    ) {
        args.entities[sender].stunTimer_ = 2;
        args.entities[reciever].stunTimer_ = 2;
        recieverCooldown = 30;
        MeteorSlowdown(
            args.velocityComponent.velocity[sender],
            args.colliderComponent.sendMeteorCooldown[sender],
            args.colliderComponent.consecutiveMeteorSends[sender]
        );
        for (int j = 0; j < MAX_RECIEVE_METEOR_BEHAVIORS; j++) {
            if (recieverBehaviors.test(j))
                meteorBehaviorFuncs[j](args, sender, reciever);
        }
    } 

    if (
        args.entities[sender].MatchesKey(projectileKey) &&
        args.projectileComponent.active[sender] &&
        recieverProps.test(RecieveProjectile)
    ) {
        args.entities[sender].stunTimer_ = 3;
        args.entities[reciever].stunTimer_ = 3;
        ProjectileSystem::HandleContact(
            args.meterComponent,
            args.projectileComponent,
            args.transformComponent,
            args.velocityComponent,
            sender,
            reciever
        );
        args.meterComponent.meter[reciever] -= args.projectileComponent.damage[sender];  
    }

    if (
        senderCooldown == 0 && // Check for cooldown to prevent pushing from a meteored object
        senderProps.test(SendPush) && 
        recieverProps.test(RecievePush)
    ) {
        // To prevent rising from collision, only push with the x and z components
        args.transformComponent.transform[reciever].position += vec3(resolutionVec.x, 0.0f, resolutionVec.z); 
    }
}

void CollisionSystem::Execute(
    Entity* entities,
    SeedManager& seedManager,
    SpreadManager& spreadManager,
    ColliderComponent& colliderComponent,
    MeterComponent& meterComponent,
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

        int& recieveCooldown = colliderComponent.recieveMeteorCooldown[i];
        if (recieveCooldown > 0)
            recieveCooldown--;

        int& sendCooldown = colliderComponent.sendMeteorCooldown[i];
        if (sendCooldown > 0)
            sendCooldown--;

        if (sendCooldown == 0)
            colliderComponent.consecutiveMeteorSends[i] = 0;
    }

    CollisionArgs args = {
        entities, 
        seedManager, 
        spreadManager, 
        colliderComponent, 
        meterComponent,
        projectileComponent, 
        transformComponent, 
        velocityComponent
    };
    Collision collisions[MAX_COLLISIONS];
    int numCollisions = GetCollisions(args, collisions);

    for (int i = 0; i < numCollisions; i++) {
        HandleCollision(args, collisions[i].entity1, collisions[i].entity2, collisions[i].resolutionVec);
        HandleCollision(args, collisions[i].entity2, collisions[i].entity1, -collisions[i].resolutionVec);
    }
}
