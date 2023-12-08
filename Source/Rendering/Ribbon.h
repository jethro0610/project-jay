#pragma once
#include <vector_const.h>
#include "SkeletonConstants.h"

struct RibbonDesc {
    int start;
    int end;
    float returnSpeed;
    float tailPower;
    float tailRatio;
};
typedef vector_const<RibbonDesc, SkeletonConstants::MAX_RIBBONS> Ribbons;
