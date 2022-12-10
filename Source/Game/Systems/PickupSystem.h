#pragma once
#include "../Entity/Entity.h"
#include "../Components/PickupComponent.h"
#include "../Components/FlagsComponent.h"

namespace PickupSystem {
    void Execute(Entity* entities, PickupComponent& pickupComponent, FlagsComponent& flagComponent);
}
