#pragma once
#include <array>
#include "../Entity/EntityLimits.h"
class Entity;
class GroundTraceComponent;
class TransformComponent;
class VelocityComponent;
class World;

namespace GroundStickSystem {
    void Stick(
        std::array<Entity, MAX_ENTITIES>& entities, 
        World& world, 
        GroundTraceComponent& groundTraceComponent,
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent
    );
}
