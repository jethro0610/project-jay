#pragma once
#include <array>
#include "../Entity/EntityLimits.h"
class Entity;
class SpreadManager;
class SpreadDetectComponent;
class TransformComponent;

namespace SpreadDetectSystem {
    void Execute(
        std::array<Entity, MAX_ENTITIES>& entities,
        SpreadManager& spreadManager,
        SpreadDetectComponent& spreadDetectComponent,
        TransformComponent& transformComponent
    );
}
