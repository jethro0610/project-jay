#pragma once
#include "../Entity/EntityManager.h"
#include "../Components/MovementComponent.h"
#include "../Components/PickupComponent.h"
#include "../Components/SpreadActivatorComponent.h"
#include "../Camera.h"
#include "../../Types/Inputs.h"

namespace PlayerInputSystem {
    void Execute(
        EntityManager& entityManager,
        Camera& camera,
        Inputs inputs, 
        MovementComponent& movementComponent,
        PickupComponent& pickupComponent,
        SpreadActivatorComponent& spreadActivatorComponent
    );
}
