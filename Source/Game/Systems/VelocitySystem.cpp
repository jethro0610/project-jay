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

void VelocitySystem::CalculateGravity(
    Entity* entities,
    VelocityComponent& velocityComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.ShouldUpdate())
            continue;
        if (!entity.MatchesKey(key))
            continue;

        vec3& velocity = velocityComponent.velocity[i];
        if (velocityComponent.useGravity[i]) {
            velocity.y -= GRAVITY_ACCELERATION;
            velocity.y = -min(-velocity.y, MAX_GRAVITY);
        }
    }
}

void VelocitySystem::Apply(
    Entity* entities,
    TransformComponent& transformComponent,
    VelocityComponent& velocityComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.ShouldUpdate())
            continue;
        if (!entity.MatchesKey(key))
            continue;

        vec3& velocity = velocityComponent.velocity[i];
        quat& angularVelocity = velocityComponent.angularVelocity[i];

        transformComponent.transform[i].position += velocity * TIMESTEP;
        transformComponent.transform[i].rotation = angularVelocity * transformComponent.transform[i].rotation;
    }
}
