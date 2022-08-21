#pragma once
#include "../World.h"
#include "../Components/ActiveEntityComponents.h"
#include "../Components/TransformComponents.h"
#include "../Components/ColliderComponents.h"

namespace CollisionSystem {
    void Execute(
        World* world,
        ActiveEntityComponents& activeComponents,
        TransformComponents& transformComponents,
        ColliderComponents& colliderComponents,
        float deltaTime
    );
}

