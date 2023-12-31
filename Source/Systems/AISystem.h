#pragma once
#include "SystemInc.h"

class Terrain;

namespace AISystem {
    void InitAIs(
        EntityList& entities,
        ComponentList& components,
        Terrain& terrain
    );

    void Execute(
        EntityList& entities,
        ComponentList& components,
        Terrain& terrain
    );
}
