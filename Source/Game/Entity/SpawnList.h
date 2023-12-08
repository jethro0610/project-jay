#pragma once
#include <nlohmann/json.hpp>
#include <vector_const.h>
#include "../../Types/Transform.h"

struct Spawn {
    nlohmann::json* description;
    Transform transform;
};
typedef vector_const<Spawn, 512> SpawnList;
