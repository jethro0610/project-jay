#pragma once
#include "../Components/DesiredMovementComponents.h"
#include "../Components/GroundTraceComponents.h"
#include "../Components/TransformComponents.h"
#include "../Components/VelocityComponents.h"
#include "../Components/ColliderComponents.h"
namespace MovementSystem {
    void Execute(
        float deltaTime,
        DesiredMovementComponents& desiredMovementComponents,
        GroundTraceComponents& groundTraceComponents,
        TransformComponents& transformComponents,
        VelocityComponents& velocityComponents,
        ColliderComponents& colliderComponents
    );
}