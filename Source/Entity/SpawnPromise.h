#pragma once
#include "EntityID.h"

struct SpawnPromise {
    bool waiting;
    bool spawned;
    EntityID id;
};
