#pragma once
#include <vector_const.h>
#include "EntityID.h"
struct DestroyInfo {
    EntityID entityId;
    bool doDestroyEffects;
};
typedef vector_const<DestroyInfo, 512> DestroyList;
