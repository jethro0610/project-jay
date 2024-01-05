#pragma once
#include "EntityTypes.h"
#include "Logging/Logger.h"

struct EntityUnion {
    EntityUnion() {
        typeId = 0;
    }
    union {
        #define ENTITYEXP(TYPE, VAR) TYPE VAR;
        EXPANDENTITIES
        #undef ENTITYEXP
    };
    EntityS::TypeID typeId;
};
