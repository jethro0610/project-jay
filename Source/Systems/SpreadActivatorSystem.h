#pragma once
#include "Systems/SystemInc.h"
class SpreadManager;
class Terrain;

namespace SpreadActivatorSystem {
    void Execute(
        EntityList& entities, 
        ComponentList& components,
        SpreadManager& spreadManager,
        Terrain& terrain
    );
}

