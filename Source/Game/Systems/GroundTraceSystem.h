#pragma once
#include "SystemInc.h"
class World;

namespace GroundTraceSystem {
    void Execute(
        EntityList& entities,
        ComponentList& components,
        World& world
    );
}
