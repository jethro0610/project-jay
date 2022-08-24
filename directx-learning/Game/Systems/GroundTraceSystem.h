#pragma once
#include "../Entity/Entity.h"
#include "../Components/GroundTraceComponent.h"
#include "../Components/TransformComponent.h"
#include "../World.h"

namespace GroundTraceSystem {
    void Execute(
        World* world, 
        Entity* entities,
        TransformComponent& transformComponent, 
        GroundTraceComponent& groundTraceComponent
    );
}