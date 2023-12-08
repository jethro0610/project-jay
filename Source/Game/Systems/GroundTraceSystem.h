#pragma once
#include "SystemInc.h"
#include "../Entity/DestroyList.h"
class Terrain;

namespace GroundTraceSystem {
    void Execute(
        EntityList& entities,
        ComponentList& components,
        DestroyList& destroyList,
        Terrain& terrain 
    );
}
