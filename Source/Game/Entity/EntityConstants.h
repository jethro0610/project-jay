#pragma once
#include "../../Types/Transform.h"

const uint16_t MAX_ENTITIES = 1024;
typedef uint16_t EntityID;
typedef std::tuple<EntityID, Transform&> EntityReturn;
