#pragma once
#include "SystemInc.h"
class EntityManager;
class SeedManager;

namespace IntervalSpawnSystem {
    void Execute(
        EntityList& entities,
        ComponentList& components,
        EntityManager& entityManager, // TODO: Create a list for spawning instead
        SeedManager& seedManager
    );
};
