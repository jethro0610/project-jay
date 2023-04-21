#pragma once
class Entity;
class TransformComponent;
class VelocityComponent;

namespace VelocitySystem {
    void Apply(
        Entity* entities,
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent
    );
}
