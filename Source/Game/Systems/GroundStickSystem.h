#pragma once
class Entity;
class GroundTraceComponent;
class TransformComponent;
class VelocityComponent;
class World;

namespace GroundStickSystem {
    void Stick(
        Entity* entities, 
        World& world, 
        GroundTraceComponent& groundTraceComponent,
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent
    );
}
