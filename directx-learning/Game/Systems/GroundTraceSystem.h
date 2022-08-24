#pragma once
#include "../Entity/Entity.h"
#include "../Components/GroundTraceComponents.h"
#include "../Components/TransformComponents.h"
#include "../World.h"

namespace GroundTraceSystem {
    void Execute(
        World* world, 
        Entity* entities,
        TransformComponents& transformComponents, 
        GroundTraceComponents& groundTraceComponents
    );
}