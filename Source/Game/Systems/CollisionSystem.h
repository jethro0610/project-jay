#pragma once
#include "../World.h"
#include "../Entity/Entity.h"
#include "../Components/TransformComponent.h"
#include "../Components/ColliderComponent.h"
#include "../Components/GroundTraceComponent.h"

const float GROUND_CUTOFF_SCALE = 0.25f;

namespace CollisionSystem {
    void Execute(
        World* world,
        Entity* entities,
        TransformComponent& transformComponent,
        ColliderComponent& colliderComponent,
        GroundTraceComponent& groundTraceComponent
    );
}

