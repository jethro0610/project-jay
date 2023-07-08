#include "CollisionSystem.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityKey.h"
#include "../World/SeedManager.h"
#include "../World/SpreadManager.h"
#include "../Components/ColliderComponent.h"
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
        if (!entity1.alive_)
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
            if (!entity2.alive_)
                continue;
            if (!entity2.MatchesKey(key))
                continue;

            vec3 entity2Pos = args.transformComponent.transform[j].position;    
            float entity2Radius = args.colliderComponent.radius0[j]; 
            entity2Radius *= args.transformComponent.transform[j].scale.x;
            
            float dist = distance(entity1Pos, entity2Pos);
            if (dist < entity1Radius + entity2Radius) {
                Collision collision = { i, j };
                collisions[numCollisions++] = collision;
            }
        }
    }
    ASSERT((numCollisions < MAX_COLLISIONS), "Exceeded max collisions");
    return numCollisions;
}

typedef void (MeteoredBehaviorFunc)(CollisionArgs args, EntityID sender, EntityID reciever);
void MeteoredLaunch(CollisionArgs args, EntityID sender, EntityID reciever) {
    DEBUGLOG("Launching entity " << reciever);
    ProjectileSystem::Launch(
        args.projectileComponent, 
        args.transformComponent, 
        args.velocityComponent, 
        reciever, 
        NULL_ENTITY
    );
}
MeteoredBehaviorFunc* meteorBehaviorFuncs[MAX_METEORED_BEHAVIORS] = {
    &MeteoredLaunch
};

void HandleCollision(
    CollisionArgs args,
    EntityID sender,
    EntityID reciever 
) {
    std::bitset<MAX_COLLIDER_PROPERTIES> senderProps = args.colliderComponent.properties[sender];
    std::bitset<MAX_METEORED_BEHAVIORS> recieverBehaviors = args.colliderComponent.meteoredBehaviors[reciever];
    int& recieverCooldown = args.colliderComponent.meteoredCooldown[reciever]; 

    if (senderProps.test(Meteor) && recieverCooldown == MAX_METEORED_COOLDOWN) {
        for (int j = 0; j < MAX_METEORED_BEHAVIORS; j++) {
            if (recieverBehaviors.test(j)) {
                meteorBehaviorFuncs[j](args, sender, reciever);
                recieverCooldown = 0;
            }
        }
    } 
}

void CollisionSystem::Execute(
    Entity* entities,
    SeedManager& seedManager,
    SpreadManager& spreadManager,
    ColliderComponent& colliderComponent,
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

        int& cooldown = colliderComponent.meteoredCooldown[i];
        if (cooldown < MAX_METEORED_COOLDOWN)
            cooldown++;
    }

    CollisionArgs args = {
        entities, 
        seedManager, 
        spreadManager, 
        colliderComponent, 
        projectileComponent, 
        transformComponent, 
        velocityComponent
    };
    Collision collisions[MAX_COLLISIONS];
    int numCollisions = GetCollisions(args, collisions);

    for (int i = 0; i < numCollisions; i++) {
        HandleCollision(args, collisions[i].entity1, collisions[i].entity2);
        HandleCollision(args, collisions[i].entity2, collisions[i].entity1);
    }
}
