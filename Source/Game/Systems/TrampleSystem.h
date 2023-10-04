#pragma once
#include <array>
#include "../Entity/EntityLimits.h"
class Entity;
class SpreadManager;
class GroundTraceComponent;
class TrampleComponent;
class TransformComponent;

namespace TrampleSystem {
    void Execute(
        std::array<Entity, MAX_ENTITIES>& entities,
        SpreadManager& spreadManager,
        GroundTraceComponent& groundTraceComponent,
        TrampleComponent& trampleComponent,
        TransformComponent& transformComponent
    );
}
