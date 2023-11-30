#pragma once
#include "SystemInc.h"
#include "../Entity/DestroyList.h"
class World;

namespace GroundTraceSystem {
    void Execute(
        EntityList& entities,
        ComponentList& components,
        DestroyList& destroyList,
        World& world
    );
}
