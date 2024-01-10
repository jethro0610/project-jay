#pragma once
#include "EntityTypes.h"
#include "Logging/Logger.h"

struct EntityUnion {
    EntityUnion() {}
    union {
        #define ENTITYEXP(TYPE, VAR, ID) TYPE VAR;
        EXPANDENTITIES
        #undef ENTITYEXP
    };
};
