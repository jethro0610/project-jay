#include "CollisionSystem.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityManager.h"
#include "../Entity/EntityKey.h"
#include "../World/SeedManager.h"
#include "../World/SpreadManager.h"
#include "../Components/ColliderComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../../Constants/GameConstants.h"
using namespace glm;

constexpr EntityKey key = GetEntityKey<ColliderComponent, TransformComponent>();

void CollisionSystem::Execute(
    Entity* entities,
    EntityManager& entityManager,
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

        const vec3 position = transformComponent.transform[i].position;
        const float radius = colliderComponent.radius0[i];

        // TODO: Only check components within the same chunk
        // NOTE: There needs to be consistent checking between which components do what
        // basically skip double checks
        // Maybe do a i-j for loop on a chunk, with j starting at i instead of 0
        for (int j = i + 1; j < MAX_ENTITIES; j++) {
            if (j == i)
                continue;

            const Entity& otherEntity = entities[j];
            if (!otherEntity.alive_)
                continue;
            if (!otherEntity.MatchesKey(key))
                continue;

            const vec3 otherPosition = transformComponent.transform[j].position;    
            const float otherRadius = colliderComponent.radius0[j]; 
            
            const float dist = distance(position, otherPosition);
            if (dist < radius + otherRadius) {
                // TODO: Store the intersections instead
                HandleIntersection(
                    entities, 
                    entityManager,
                    seedManager,
                    spreadManager, 
                    colliderComponent, 
                    projectileComponent, 
                    transformComponent, 
                    velocityComponent, 
                    i, 
                    j
                );
                HandleIntersection(
                    entities, 
                    entityManager,
                    seedManager,
                    spreadManager, 
                    colliderComponent, 
                    projectileComponent, 
                    transformComponent, 
                    velocityComponent, 
                    j, 
                    i
                );
            }
        }
    }
}

constexpr EntityKey projectileKey = GetEntityKey<ProjectileComponent, VelocityComponent>();

void CollisionSystem::HandleIntersection(
    Entity* entities,
    EntityManager& entityManager,
    SeedManager& seedManager,
    SpreadManager& spreadManager,
    ColliderComponent& colliderComponent,
    ProjectileComponent& projectileComponent,
    TransformComponent& transformComponent,
    VelocityComponent& velocityComponent,
    EntityID entity1,
    EntityID entity2 
) {
    if (colliderComponent.properties[entity1].test(ColliderProperties::Meteor)) {
        const vec3 velocity = velocityComponent.velocity[entity1];
        float speed = length(vec2(velocity.x, velocity.z));
        // if (
        //     speed >= METEOR_SPEED &&
        //     bubbleComponent.properties[entity2].test(BubbleProperties::ThrowOnMeteored) && 
        //     projectileComponent.state[entity2] == ProjectileState::Inactive
        // ) {
        //     ProjectileSystem::Throw(entities, projectileComponent, transformComponent, velocityComponent, entity2, entity1, 30.0f); 
        // }

        if (
            colliderComponent.properties[entity2].test(ColliderProperties::SeedOnMeteored)
        ) {
            seedManager.CreateMultipleSeed(transformComponent.transform[entity2].position + vec3(0.0f, 0.25f, 0.0f), 64, 30);
            entityManager.DestroyEntity(entity2);
        }
    }
}
