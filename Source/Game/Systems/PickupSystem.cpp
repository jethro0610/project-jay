#include "PickupSystem.h"

void PickupSystem::Execute(Entity* entities, PickupComponent& pickupComponent, FlagsComponent& flagComponent) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;

        if (!entity.HasComponent<PickupComponent>())
            continue;

        if (pickupComponent.timer[i] > 0) {
            // DO PICKUP HERE 
        }
    }
}
