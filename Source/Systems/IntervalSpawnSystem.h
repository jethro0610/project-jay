#pragma once
#include "Systems/SystemInc.h"

class EntityManager;
class SeedManager;

namespace IntervalSpawnSystem {
    void Execute(
        EntityList& entities,
        ComponentList& components,
        SpawnList& spawnList,
        SeedManager& seedManager
    );
};