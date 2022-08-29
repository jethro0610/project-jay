#pragma once
#include "../Entity/Entity.h"
#include "../TimeConstants.h"
#include "../Components/MovementComponent.h"
#include "../Components/GroundTraceComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/ColliderComponent.h"

#define GRAVITY_ACCELERATION 1.0f
#define MAX_GRAVITY 60.0f

namespace MovementSystem {
    void Execute(
        Entity* entities,
        MovementComponent& movementComponent,
        GroundTraceComponent& groundTraceComponent,
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent,
        ColliderComponent& colliderComponent
    );

    void CalculateDefaultMovement(
        const vec3& desiredMovement, 
        const float& acceleration, 
        const float& speedDecay, 
        const float& momentumDecay,
        const float& minSpeed,
        const float& maxSpeed,
        float& speed, 
        vec3& velocity, 
        quat& rotation
    );
    void CalculateSkiMovement(
        const vec3& desiredMovement, 
        const vec3& groundNormal,
        const float& minSpeed,
        const float& maxSpeed,
        float& speed, 
        vec3& velocity, 
        quat& rotation
    );
}