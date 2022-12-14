#pragma once
#include "../Entity/Entity.h"
#include "../Components/PickupComponent.h"
#include "../Components/HoldableComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/InputComponent.h"

namespace PickupSystem {
    void ExecutePickup(
        Entity* entities, 
        PickupComponent& pickupComponent, 
        HoldableComponent& holdableComponent, 
        TransformComponent& transformComponent,
        InputComponent& inputComponent
    );

    void ExecuteHold(
        Entity* entities, 
        PickupComponent& pickupComponent, 
        TransformComponent& transformComponent,
        InputComponent& inputComponent
    );
}
