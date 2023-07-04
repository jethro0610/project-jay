#pragma once
class Entity;
class GroundTraceComponent;
class TransformComponent;
class World;

namespace GroundTraceSystem {
    void Execute(
        Entity* entities,
        World& world,
        GroundTraceComponent& groundTraceComponent,
        TransformComponent& transformComponent
    );
}
