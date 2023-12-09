#pragma once
#include "Systems/SystemInc.h"

namespace VelocitySystem {
    void CalculateGravity(
        EntityList& entities,
        ComponentList& components
    );

    void Apply(
        EntityList& entities,
        ComponentList& components
    );
}
