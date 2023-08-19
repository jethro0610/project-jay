#pragma once
#include <array>
#include "../Entity/EntityLimits.h"
class Entity;
class SpreadManager;
class World;
class GroundTraceComponent;
class MeterComponent;
class SpreadActivatorComponent;
class SpreadDetectComponent;
class TransformComponent;

namespace SpreadActivatorSystem {
    void Execute(
        std::array<Entity, MAX_ENTITIES>& entities, 
        SpreadManager& spreadManager,
        World& world,
        GroundTraceComponent& groundTraceComponent,
        MeterComponent& meterComponent,
        SpreadActivatorComponent& spreadActivatorComponent,
        SpreadDetectComponent& spreadDetectComponent,
        TransformComponent& transformComponent
    );
}

