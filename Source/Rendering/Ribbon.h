#pragma once
#include <vector_const.h>

struct RibbonDesc {
    static constexpr int MAX_RIBBONS = 4;
    static constexpr  int MAX_RIBBON_CHAINS = 4;

    int start;
    int end;
    float returnSpeed;
    float tailPower;
    float tailRatio;
};
typedef vector_const<RibbonDesc, RibbonDesc::MAX_RIBBONS> Ribbons;
