#pragma once
#include "../Entity/Entity.h"
#include "../Components/MovementComponent.h"
#include "../Components/PickupComponent.h"
#include "../Components/SpreadActivatorComponent.h"
#include "../Camera.h"
#include "../../Types/Inputs.h"

namespace PlayerInputSystem {
    void Execute(
        Inputs inputs, 
        Camera* camera, 
        Entity* entities, 
        MovementComponent& movementComponent,
        PickupComponent& pickupComponent,
        SpreadActivatorComponent& spreadActivatorComponent
    );
}
