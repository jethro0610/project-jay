#pragma once
#include "Systems/SystemInc.h"

namespace TransformSystem {
    void UpdateLastTransforms(
        EntityList& entities,
        ComponentList& components
    );
    void UpdateRenderTransforms(
        EntityList& entities,
        ComponentList& components,
        float interpTime
    );
}
