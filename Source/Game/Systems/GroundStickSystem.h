#pragma once
class Entity;
class GroundTraceComponent;
class TransformComponent;
class World;

namespace GroundStickSystem {
    void Step(
        Entity* entities, 
        World& world, 
        GroundTraceComponent& groundTraceComponent,
        TransformComponent& transformComponent
    );

    void Stick(
        Entity* entities, 
        World& world, 
        GroundTraceComponent& groundTraceComponent,
        TransformComponent& transformComponent
    );
}
