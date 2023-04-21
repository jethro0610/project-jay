#pragma once
class Entity;
class GroundTraceComponent;
class TransformComponent;
class World;
class VelocityComponent;

namespace GroundTraceSystem {
    void Execute(
        Entity* entities,
        World& world,
        GroundTraceComponent& groundTraceComponent,
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent
    );
}
