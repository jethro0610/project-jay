#pragma once
#include "SystemInc.h"
class SpreadManager;
class World;

namespace SpreadActivatorSystem {
    void Execute(
        EntityList& entities, 
        ComponentList& components,
        SpreadManager& spreadManager,
        World& world
    );
}

