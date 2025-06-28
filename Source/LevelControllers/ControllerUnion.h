#pragma once
#include "LevelControllerIds.h"

struct ControllerUnion {
    ControllerUnion(
        EntityList& entities,
        SeedManager& seedManager,
        Terrain& terrain
    ) {
        base.id_ = -1;
        base.entities_ = &entities;
        base.seedManager_ = &seedManager;
        base.terrain_ = &terrain;
    }
    union {
        #define LVCONTROLLER(TYPE, VAR, ID) TYPE VAR;
        EXPANDCONTROLLERS
        #undef LVCONTROLLER
    };
};
