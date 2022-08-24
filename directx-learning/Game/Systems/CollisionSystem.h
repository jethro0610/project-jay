#pragma once
#include "../World.h"
#include "../Entity/Entity.h"
#include "../Components/TransformComponents.h"
#include "../Components/ColliderComponents.h"
#include "../Components/GroundTraceComponents.h"

#define GROUND_CUTOFF_SCALE 0.25f

namespace CollisionSystem {
    void Execute(
        World* world,
        Entity* entities,
        TransformComponents& transformComponents,
        ColliderComponents& colliderComponents,
        GroundTraceComponents& groundTraceComponents
    );
}

