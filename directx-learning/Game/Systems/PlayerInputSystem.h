#pragma once
#include "../Components/DesiredMovementComponents.h"
#include "../Camera.h"
#include "../../Types/Inputs.h"

namespace PlayerInputSystem {
    void Execute(Inputs inputs, Camera* camera, DesiredMovementComponents& desiredMovementComponents);
}

