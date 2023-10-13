#pragma once
#include "SystemInc.h"
#include "../Entity/SpawnList.h"

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
