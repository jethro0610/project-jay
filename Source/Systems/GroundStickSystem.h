#pragma once
#include "Systems/SystemInc.h"
class Terrain;

namespace GroundStickSystem {
    void Stick(
        EntityList& entities, 
        ComponentList& components,
        Terrain& world
    );
}
