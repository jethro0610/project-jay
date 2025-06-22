#pragma once
#include "LevelControllerIds.h"

struct ControllerUnion {
    ControllerUnion(
        Clock& clock,
        EntityList& entities,
        ScoreKeeper& scoreKeeper,
        SeedManager& seedManager,
        Terrain& terrain
    ) {
        base.id_ = -1;
        base.clock_ = &clock;
        base.entities_ = &entities;
        base.scoreKeeper_ = &scoreKeeper;
        base.seedManager_ = &seedManager;
        base.terrain_ = &terrain;
    }
    union {
        #define LVCONTROLLER(TYPE, VAR, ID) TYPE VAR;
        EXPANDCONTROLLERS
        #undef LVCONTROLLER
    };
};
