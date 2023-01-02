#pragma once
#include "../Entity/Entity.h"
#include "../Components/PickupComponent.h"
#include "../Components/HoldableComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/ProjectileComponent.h"

namespace PickupSystem {
    void ExecutePickup(
        Entity* entities, 
        PickupComponent& pickupComponent, 
        HoldableComponent& holdableComponent, 
        TransformComponent& transformComponent
    );

    void ExecuteHold(
        Entity* entities, 
        PickupComponent& pickupComponent, 
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent,
        ProjectileComponent& projectileComponent
    );
}
