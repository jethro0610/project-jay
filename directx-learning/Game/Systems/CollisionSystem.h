#pragma once
#include "../World.h"
#include "../Components/ActiveEntityComponents.h"
#include "../Components/TransformComponents.h"
#include "../Components/ColliderComponents.h"
#include "../Components/GroundTraceComponents.h"

#define GROUND_CUTOFF_SCALE 0.25f

namespace CollisionSystem {
    void Execute(
        float deltaTime,
        World* world,
        ActiveEntityComponents& activeComponents,
        TransformComponents& transformComponents,
        ColliderComponents& colliderComponents,
        GroundTraceComponents& groundTraceComponents
    );
}

