#pragma once
#include <array>
#include "../Entity/EntityLimits.h"
class Entity;
class GroundTraceComponent;
class TransformComponent;
class World;

namespace GroundTraceSystem {
    void Execute(
        std::array<Entity, MAX_ENTITIES>& entities,
        World& world,
        GroundTraceComponent& groundTraceComponent,
        TransformComponent& transformComponent
    );
}
