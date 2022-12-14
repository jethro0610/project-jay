#pragma once
#include "../Entity/Entity.h"
#include "../Components/InputComponent.h"
#include "../Camera.h"
#include "../../Types/Inputs.h"

namespace PlayerInputSystem {
    void Execute(Inputs inputs, Camera* camera, Entity* entities, InputComponent& inputComponent);
}
