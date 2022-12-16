#pragma once
#include "../Entity/Entity.h"
#include "../Components/GroundTraceComponent.h"
#include "../Components/TransformComponent.h"
#include "../World.h"

namespace GroundStickSystem {
    void Step (
        World* world, 
        Entity* entities,
        TransformComponent& transformComponent, 
        GroundTraceComponent& groundTraceComponent
    );

    void Stick (
        World* world, 
        Entity* entities,
        TransformComponent& transformComponent, 
        GroundTraceComponent& groundTraceComponent
    );
}
