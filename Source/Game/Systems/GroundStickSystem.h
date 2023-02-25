#pragma once
#include "../Entity/EntityManager.h"
#include "../Components/GroundTraceComponent.h"
#include "../Components/TransformComponent.h"
#include "../World.h"

namespace GroundStickSystem {
    void Step (
        EntityManager& entityManager,
        World& world, 
        TransformComponent& transformComponent, 
        GroundTraceComponent& groundTraceComponent
    );

    void Stick (
        EntityManager& entityManager,
        World& world, 
        TransformComponent& transformComponent, 
        GroundTraceComponent& groundTraceComponent
    );
}
