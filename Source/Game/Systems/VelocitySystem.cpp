#include "VelocitySystem.h"
#include "../TimeConstants.h"

void VelocitySystem::Apply(
    EntityManager& entityManager,
    TransformComponent& transformComponent,
    VelocityComponent& velocityComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entityManager.entities_[i];
        if (!entity.alive_)
            continue;

        if (!entity.HasComponents({transformComponent, velocityComponent}))
            continue;

        transformComponent.transform[i].position_ += velocityComponent.velocity[i] * TIMESTEP;
    }
}
