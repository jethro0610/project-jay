#include "PickupSystem.h"
#include "ProjectileSystem.h" 
using namespace glm;

bool PickupSystem::DoPickup(int pickupEntityId, int holdEntityId, PickupComponent& pickupComponent) {
    if (!pickupComponent.pickup[pickupEntityId])
        return false;
    if (pickupComponent.entityId[pickupEntityId] != -1)
        return false;

    pickupComponent.entityId[pickupEntityId] = holdEntityId;
    return true;
}

void PickupSystem::ExecuteHold(
    EntityManager& entityManager, 
    PickupComponent& pickupComponent, 
    TransformComponent& transformComponent,
    VelocityComponent& velocityComponent,
    ProjectileComponent& projectileComponent
) {
    const Entity* entities = entityManager.entities_;
    for (uint16_t i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;
        if (!entity.HasComponent(pickupComponent))
            continue;
        if (pickupComponent.entityId[i] == -1)
            continue;

        const int& holdEntityId = pickupComponent.entityId[i]; 
        const Transform& transform = transformComponent.transform[i];
        transformComponent.transform[holdEntityId].position_ = transform.position_;
        transformComponent.transform[holdEntityId].rotation_ = transform.rotation_;

        if (!pickupComponent.pickup[i]) {
            if (!entities[holdEntityId].HasComponent(velocityComponent))
                continue;
            ProjectileSystem::Throw(
                projectileComponent, 
                velocityComponent, 
                transformComponent,
                holdEntityId,
                i,
                30.0f
            );
            pickupComponent.entityId[i] = -1;
        }
    }
}
