#pragma once
#include "EntityID.h"
#include <vector_const.h>

struct DestroyInfo {
    EntityID entityId;
    bool doDestroyEffects;
};
typedef vector_const<DestroyInfo, 512> DestroyList;
