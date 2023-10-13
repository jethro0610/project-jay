#pragma once
#include "SystemInc.h"

namespace SkeletonSystem {
    void CalculatePoses(
        EntityList& entities,
        ComponentList& components
    );

    void InterpPoses(
        EntityList& entities,
        ComponentList& components,
        float interpTime
    );
}
