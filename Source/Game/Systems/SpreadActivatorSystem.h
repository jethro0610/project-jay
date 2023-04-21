#pragma once
#include "../../Types/EntityTypes.h"
class Entity;
class SpreadManager;
class World;
class GroundTraceComponent;
class SpreadActivatorComponent;
class SpreadDetectComponent;
class TransformComponent;

namespace SpreadActivatorSystem {
    void Execute(
        Entity* entities, 
        SpreadManager& spreadManager,
        World& world,
        GroundTraceComponent& groundTraceComponent,
        SpreadActivatorComponent& spreadActivatorComponent,
        SpreadDetectComponent& spreadDetectComponent,
        TransformComponent& transformComponent
    );
}

