#pragma once
#include "SystemInc.h"
#include "../Entity/EntityID.h"
class World;

namespace ProjectileSystem {
    void Execute(
        EntityList& entities, 
        ComponentList& components
    );
}
