#pragma once
#include "Systems/SystemInc.h"
class Terrain;

namespace GroundTraceSystem {
    void Execute(
        EntityList& entities,
        ComponentList& components,
        DestroyList& destroyList,
        Terrain& terrain 
    );
}
