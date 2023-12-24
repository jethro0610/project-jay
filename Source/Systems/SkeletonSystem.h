#pragma once
#include "Systems/SystemInc.h"

namespace SkeletonSystem {
    void CalculatePoses(
        EntityList& entities,
        ComponentList& components
    );

    void CalculateBasePoses(
        EntityList& entities,
        ComponentList& components
    );

    void InterpPoses(
        EntityList& entities,
        ComponentList& components,
        float interpTime
    );
}
