#pragma once
#include "../World/World.h"
#include "../Entity/EntityManager.h"
#include "../Components/TransformComponent.h"
#include "../Components/WorldColliderComponent.h"

const float GROUND_CUTOFF_SCALE = 0.25f;

namespace CollisionSystem {
    void Execute(
        EntityManager& entityManager,
        World& world,
        TransformComponent& transformComponent,
        WorldColliderComponent& worldColliderComponent
    );
}

