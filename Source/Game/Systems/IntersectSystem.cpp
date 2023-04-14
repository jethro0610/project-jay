#include "../../Constants/GameConstants.h"
#include "./IntersectSystem.h"
#include "../Systems/PickupSystem.h"
#include "../Systems/ProjectileSystem.h"
using namespace glm;

void IntersectSystem::Execute(
    EntityManager& entityManager,
    SpreadManager& spreadManager
) {
    const Entity* entities = entityManager.entities_;
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        TransformComponent& transformComponent = entityManager.transformComponent_;
        BubbleComponent& bubbleComponent = entityManager.bubbleComponent_;

        if (!entity.alive_)
            continue;
        if (!entity.HasComponents({transformComponent, bubbleComponent}))
            continue;
        const vec3 position = transformComponent.transform[i].position_;
        const float radius = bubbleComponent.radius[i];

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
            if (!entity.HasComponents({transformComponent, bubbleComponent}))
                continue;

            const vec3 otherPosition = transformComponent.transform[j].position_;    
            const float otherRadius = bubbleComponent.radius[j]; 
            
            const float dist = distance(position, otherPosition);
            if (dist < radius + otherRadius) {
                HandleIntersection(
                    entityManager,
                    spreadManager, 
                    i, 
                    j 
                );

                HandleIntersection(
                    entityManager,
                    spreadManager, 
                    j, 
                    i 
                );
            }
        }
    }
}

void IntersectSystem::HandleIntersection(
    EntityManager& entityManager,
    SpreadManager& spreadManager,
    EntityID entity1,
    EntityID entity2 
) {
    const Entity* entities = entityManager.entities_;
    PickupComponent& pickupComponent = entityManager.pickupComponent_; 
    HoldableComponent& holdableComponent = entityManager.holdableComponent_; 
    TransformComponent& transformComponent = entityManager.transformComponent_;
    ProjectileComponent& projectileComponent = entityManager.projectileComponent_;
    VelocityComponent& velocityComponent = entityManager.velocityComponent_;
    BubbleComponent& bubbleComponent = entityManager.bubbleComponent_;

    if (entities[entity1].HasComponent(pickupComponent) && 
        entities[entity2].HasComponent(holdableComponent)) 
    {
        PickupSystem::TryPickup(entity1, entity2, pickupComponent);
    }
    
    if (bubbleComponent.properties[entity1].test(BubbleProperties::Meteor)) {
        const vec3 velocity = velocityComponent.velocity[entity1];
        float speed = length(vec2(velocity.x, velocity.z));
        if (
            speed >= METEOR_SPEED &&
            bubbleComponent.properties[entity2].test(BubbleProperties::ThrowOnMeteored) && 
            projectileComponent.state[entity2] == ProjectileState::Inactive
        ) {
            ProjectileSystem::Throw(projectileComponent, velocityComponent, transformComponent, entity2, entity1, 30.0f); 
        }
    }
}
