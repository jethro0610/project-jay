#include "PickupSystem.h"
#include "ProjectileSystem.h" 
using namespace glm;

void PickupSystem::ExecutePickup(
    Entity* entities, 
    PickupComponent& pickupComponent, 
    HoldableComponent& holdableComponent,
    TransformComponent& transformComponent,
    BubbleComponent& bubbleComponent
) {
    for (uint16_t i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;

        if (!entity.HasComponent<PickupComponent>())
            continue;

        if (!pickupComponent.pickup[i])
            continue;

        // Find an entity to pick up
        vec3 entityPos = transformComponent.transform[i].position_;
        int& holdEntityId = pickupComponent.entityId[i];
        if (holdEntityId != -1)
            continue;

        for (uint16_t e = 0; e < MAX_ENTITIES; e++) {
            const Entity& other = entities[e];
            if (!other.alive_)
                continue;
            if (!other.HasComponent<HoldableComponent>())
                continue;

            // NOTE: This only picks up the first entity in range. May need to keep track of the closest instead
            // Also need to use entity partitions when implemented
            vec3 otherPos = transformComponent.transform[e].position_;
            if (distance(entityPos, otherPos) <= bubbleComponent.radius[i] + bubbleComponent.radius[e]) {
                holdEntityId = e;
               break; 
            }
        }
    }
}

void PickupSystem::ExecuteHold(
    Entity* entities, 
    PickupComponent& pickupComponent, 
    TransformComponent& transformComponent,
    VelocityComponent& velocityComponent,
    ProjectileComponent& projectileComponent
) {
    for (uint16_t i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;
        if (!entity.HasComponent<PickupComponent>())
            continue;
        if (pickupComponent.entityId[i] == -1)
            continue;

        const int& holdEntityId = pickupComponent.entityId[i]; 
        if (!entities[holdEntityId].HasComponent<TransformComponent>())
            continue;
        const Transform& transform = transformComponent.transform[i];
        transformComponent.transform[holdEntityId].position_ = transform.position_;
        transformComponent.transform[holdEntityId].rotation_ = transform.rotation_;

        if (!pickupComponent.pickup[i]) {
            if (!entities[holdEntityId].HasComponent<VelocityComponent>())
                continue;
            ProjectileSystem::Launch(
                holdEntityId, 
                -1, 
                velocityComponent.velocity[i], 
                projectileComponent, 
                velocityComponent, 
                transformComponent
            );
            pickupComponent.entityId[i] = -1;
        }
    }
}
