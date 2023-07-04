#include "VelocitySystem.h"
#include <glm/vec3.hpp>
#include "../Entity/Entity.h"
#include "../Entity/EntityKey.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../../Constants/TimeConstants.h"
#include "../../Logging/ScreenText.h"
using namespace glm;

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

        transformComponent.transform[i].position += velocityComponent.velocity[i] * TIMESTEP;
    }
}
