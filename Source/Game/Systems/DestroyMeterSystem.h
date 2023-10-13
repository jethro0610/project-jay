#pragma once
#include "SystemInc.h"

class EntityManager;
class SeedManager;

namespace DestroyMeterSystem {
    void Execute(
        EntityList& entities,
        ComponentList& components,
        EntityManager& entityManager,
        SeedManager& seedManager
    );
}
