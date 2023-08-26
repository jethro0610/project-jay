#pragma once
#include <vector_const.h>
#include "RenderConstants.h"

struct RibbonDesc {
    int start;
    int end;
    float returnSpeed;
    float tailPower;
    float tailRatio;
};
typedef vector_const<RibbonDesc, MAX_RIBBONS> Ribbons;
