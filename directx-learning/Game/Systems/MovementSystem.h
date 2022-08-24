#pragma once
#include "../Entity/Entity.h"
#include "../TimeConstants.h"
#include "../Components/DesiredMovementComponents.h"
#include "../Components/GroundTraceComponents.h"
#include "../Components/TransformComponents.h"
#include "../Components/VelocityComponents.h"
#include "../Components/ColliderComponents.h"

#define MAX_SPEED 15.0f
#define FRICTION 0.1f
#define SPEED_DECAY (1.0f - FRICTION)
#define ACCELERATION ((MAX_SPEED / SPEED_DECAY) - MAX_SPEED)

#define GRAVITY_ACCELERATION 1.0f
#define MAX_GRAVITY 60.0f

namespace MovementSystem {
    void Execute(
        Entity* entities,
        DesiredMovementComponents& desiredMovementComponents,
        GroundTraceComponents& groundTraceComponents,
        TransformComponents& transformComponents,
        VelocityComponents& velocityComponents,
        ColliderComponents& colliderComponents
    );
}