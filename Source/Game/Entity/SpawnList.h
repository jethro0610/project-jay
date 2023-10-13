#pragma once
#include <string>
#include <vector_const.h>
#include "../../Types/Transform.h"

struct Spawn {
    std::string name;
    Transform transform;
};
typedef vector_const<Spawn, 512> SpawnList;
