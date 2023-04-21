#include "PickupSystem.h"
#include "../Entity/EntityManager.h"
#include "ProjectileSystem.h" 
using namespace glm;

bool PickupSystem::TryPickup(EntityManager& entityManager, int pickupEntityId, int holdEntityId) {
    PickupComponent& pickupComponent = entityManager.pickupComponent_;
    if (!pickupComponent.pickup[pickupEntityId])
        return false;
    if (pickupComponent.entityId[pickupEntityId] != -1)
        return false;

    pickupComponent.entityId[pickupEntityId] = holdEntityId;
    return true;
}

void PickupSystem::ExecuteHold(EntityManager& entityManager) {
    const Entity* entities = entityManager.entities_;
    PickupComponent& pickupComponent = entityManager.pickupComponent_;
    TransformComponent& transformComponent = entityManager.transformComponent_;
    VelocityComponent& velocityComponent = entityManager.velocityComponent_;
    ProjectileComponent& projectileComponent = entityManager.projectileComponent_;
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
            ProjectileSystem::Throw(entityManager, holdEntityId, i, 30.0f);
            pickupComponent.entityId[i] = -1;
        }
    }
}
