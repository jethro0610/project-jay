#pragma once
#include "../Entity/Entity.h"
#include "../TimeConstants.h"
#include "../Components/DesiredMovementComponent.h"
#include "../Components/GroundTraceComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/ColliderComponent.h"

#define MAX_SPEED 15.0f
#define FRICTION 0.1f
#define SPEED_DECAY (1.0f - FRICTION)
#define ACCELERATION ((MAX_SPEED / SPEED_DECAY) - MAX_SPEED)

#define GRAVITY_ACCELERATION 1.0f
#define MAX_GRAVITY 60.0f

namespace MovementSystem {
    void Execute(
        Entity* entities,
        DesiredMovementComponent& desiredMovementComponent,
        GroundTraceComponent& groundTraceComponent,
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent,
        ColliderComponent& colliderComponent
    );
}