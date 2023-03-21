#pragma once
#include "../Entity/EntityManager.h"
#include "../Components/PickupComponent.h"
#include "../Components/HoldableComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/BubbleComponent.h"

namespace PickupSystem {
    bool TryPickup(int pickupEntityId, int holdEntityId, PickupComponent& pickupComponent);

    void ExecuteHold(
        EntityManager& entityManager,
        PickupComponent& pickupComponent, 
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent,
        ProjectileComponent& projectileComponent
    );
}
