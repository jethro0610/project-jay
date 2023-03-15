#pragma once
#include "../../Types/Transform.h"

const uint16_t MAX_ENTITIES = 512;
typedef uint16_t EntityID;
typedef std::tuple<EntityID, Transform&> EntityReturn;
