#pragma once
#include "EntityDescription.h"
#include "Types/Transform.h"
#include <vector_const.h>

struct Spawn {
    EntityDescription* description;
    Transform transform;
};
typedef vector_const<Spawn, 512> SpawnList;
