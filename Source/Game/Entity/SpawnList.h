#pragma once
#include <vector_const.h>
#include "EntityDescription.h"
#include "../../Types/Transform.h"

struct Spawn {
    EntityDescription* description;
    Transform transform;
};
typedef vector_const<Spawn, 512> SpawnList;
