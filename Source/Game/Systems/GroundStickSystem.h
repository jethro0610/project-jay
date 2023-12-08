#pragma once
#include "SystemInc.h"
class Terrain;

namespace GroundStickSystem {
    void Stick(
        EntityList& entities, 
        ComponentList& components,
        Terrain& world
    );
}
