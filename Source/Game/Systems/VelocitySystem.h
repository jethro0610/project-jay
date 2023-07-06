#pragma once
class Entity;
class TransformComponent;
class VelocityComponent;

namespace VelocitySystem {
    void CalculateGravity(
        Entity* entities,
        VelocityComponent& velocityComponent
    );

    void Apply(
        Entity* entities,
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent
    );
}
