#pragma once
#include "Entity/EntityID.h"
#include "Systems/SystemInc.h"

class Terrain;

namespace SpinRatController {
    void Init(
        EntityID entityId, 
        EntityList& entities,
        ComponentList& components,
        Terrain& terrain
    );

    void Control(
        EntityID entityId, 
        EntityList& entities,
        ComponentList& components,
        Terrain& terrain
    );
};
