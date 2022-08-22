#pragma once
#include "../Components/GroundTraceComponents.h"
#include "../Components/TransformComponents.h"
#include "../World.h"

namespace GroundTraceSystem {
    void Execute(
        World* world, 
        TransformComponents& transformComponents, 
        GroundTraceComponents& groundTraceComponents
    );
}