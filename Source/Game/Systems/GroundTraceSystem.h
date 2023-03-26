#pragma once
#include "../Entity/EntityManager.h"
#include "../Components/GroundTraceComponent.h"
#include "../Components/TransformComponent.h"
#include "../World.h"

namespace GroundTraceSystem {
    void Execute(
        EntityManager& entityManager,
        World& world
    );
}
