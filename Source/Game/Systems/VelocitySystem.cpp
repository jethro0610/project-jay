#include "VelocitySystem.h"
#include "../TimeConstants.h"

void VelocitySystem::Apply(
    Entity* entites,
    TransformComponent& transformComponent,
    VelocityComponent& velocityComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entites[i];
        if (!entity.alive_)
            continue;

        if (!entity.HasComponents<TransformComponent, VelocityComponent>())
            continue;

        transformComponent.transform[i].position_ += velocityComponent.velocity[i] * TIMESTEP;
    }
}
