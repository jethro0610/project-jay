#include "VelocitySystem.h"
#include "Components/TransformComponent.h"
#include "Components/VelocityComponent.h"
#include "Time/Time.h"
using namespace glm;

constexpr EntityKey key = GetEntityKey<TransformComponent, VelocityComponent>();

void VelocitySystem::CalculateGravity(
    EntityList& entities,
    ComponentList& components
) {
    auto& velocityComponent = components.Get<VelocityComponent>();

    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.ShouldUpdate())
            continue;
        if (!entity.MatchesKey(key))
            continue;

        vec3& velocity = velocityComponent.velocity[i];
        if (velocityComponent.useGravity[i]) {
            velocity.y -= VelocityComponent::GRAVITY_ACCELERATION;
            velocity.y = -min(-velocity.y, VelocityComponent::MAX_GRAVITY);
        }
    }
}

void VelocitySystem::Apply(
    EntityList& entities,
    ComponentList& components
) {
    auto& transformComponent = components.Get<TransformComponent>();
    auto& velocityComponent = components.Get<VelocityComponent>();

    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entities[i].ShouldUpdate(key)) continue;

        vec3& velocity = velocityComponent.velocity[i];
        quat& angularVelocity = velocityComponent.angularVelocity[i];

        transformComponent.transform[i].position += velocity * GlobalTime::TIMESTEP;
        transformComponent.transform[i].rotation = angularVelocity * transformComponent.transform[i].rotation;
    }
}