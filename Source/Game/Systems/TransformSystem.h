#pragma once
#include <array>
#include "../Entity/EntityLimits.h"
class Entity;
class TransformComponent;

namespace TransformSystem {
    void UpdateLastTransforms(
        std::array<Entity, MAX_ENTITIES>& entities,
        TransformComponent& transformComponent
    );
    void UpdateRenderTransforms(
        std::array<Entity, MAX_ENTITIES>& entities,
        TransformComponent& transformComponent,
        float interpTime
    );
}
