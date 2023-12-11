#pragma once
#include <vector_const.h>

struct RibbonDesc {
    static constexpr int MAX_RIBBONS = 4;
    static constexpr  int MAX_RIBBON_CHAINS = 4;

    RibbonDesc() {
        start = 0;
        end = 0;
        returnSpeed = 0;
        tailPower = 0;
        tailRatio = 0;
    }

    int start;
    int end;
    float returnSpeed;
    float tailPower;
    float tailRatio;
};
typedef vector_const<RibbonDesc, RibbonDesc::MAX_RIBBONS> Ribbons;
