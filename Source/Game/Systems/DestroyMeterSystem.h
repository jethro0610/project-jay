#pragma once
#include "SystemInc.h"
#include "../Entity/DestroyList.h"

class EntityManager;
class SeedManager;

namespace DestroyMeterSystem {
    void Execute(
        EntityList& entities,
        ComponentList& components,
        DestroyList& destroyList,
        SeedManager& seedManager
    );
}
