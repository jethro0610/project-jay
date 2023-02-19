#pragma once
#include "../Entity/Entity.h"
#include "../Components/PickupComponent.h"
#include "../Components/HoldableComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/BubbleComponent.h"

namespace PickupSystem {
    bool DoPickup(int pickupEntityId, int holdEntityId, PickupComponent& pickupComponent);

    void ExecuteHold(
        Entity* entities, 
        PickupComponent& pickupComponent, 
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent,
        ProjectileComponent& projectileComponent
    );
}
