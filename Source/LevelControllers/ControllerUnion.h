#pragma once
#include "LevelControllerIds.h"

struct ControllerUnion {
    ControllerUnion() {}
    union {
        #define LVCONTROLLER(TYPE, VAR, ID) TYPE VAR;
        EXPANDCONTROLLERS
        #undef LVCONTROLLER
    };
};
