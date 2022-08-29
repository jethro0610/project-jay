#pragma once
#include "../Entity/Entity.h"
#include "../Components/MovementComponent.h"
#include "../Camera.h"
#include "../../Types/Inputs.h"

namespace PlayerInputSystem {
    void Execute(Inputs inputs, Camera* camera, Entity* entities, MovementComponent& desiredMovementComponent);
}

