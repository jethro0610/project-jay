#pragma once
#include "../EntityConstants.h"
#include "../Components/DesiredMovementComponents.h"
#include "../Camera.h"
#include "../../Types/Inputs.h"

namespace PlayerInputSystem {
    void Execute(Inputs inputs, Camera* camera, Entity* entities, DesiredMovementComponents& desiredMovementComponents);
}

