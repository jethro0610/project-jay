#pragma once
#include <array>
#include "../Entity/EntityLimits.h"
class Entity;
class TransformComponent;
class VelocityComponent;

namespace VelocitySystem {
    void CalculateGravity(
        std::array<Entity, MAX_ENTITIES>& entities,
        VelocityComponent& velocityComponent
    );

    void Apply(
        std::array<Entity, MAX_ENTITIES>& entities,
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent
    );
}
