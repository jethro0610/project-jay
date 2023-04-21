#include "VelocitySystem.h"
#include "../Entity/Entity.h"
#include "../../Helpers/EntityHelpers.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../../Constants/TimeConstants.h"

constexpr EntityKey key = GetEntityKey<TransformComponent, VelocityComponent>();

void VelocitySystem::Apply(
    Entity* entities,
    TransformComponent& transformComponent,
    VelocityComponent& velocityComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;
        if (!entity.MatchesKey(key))
            continue;

        transformComponent.transform[i].position_ += velocityComponent.velocity[i] * TIMESTEP;
    }
}
