#pragma once
#include "../Entity/Entity.h"
#include "../TimeConstants.h"
#include "../Components/MovementComponent.h"
#include "../Components/GroundTraceComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/ColliderComponent.h"

const float GRAVITY_ACCELERATION = 1.0f;
const float MAX_GRAVITY = 60.0f;

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
        const glm::vec3& desiredMovement, 
        const float& acceleration, 
        const float& speedDecay, 
        const float& momentumDecay,
        const float& minSpeed,
        const float& maxSpeed,
        float& speed, 
        glm::vec3& velocity, 
        glm::quat& rotation
    );
    void CalculateSkiMovement(
        const glm::vec3& desiredMovement, 
        const glm::vec3& groundNormal,
        const float& minSpeed,
        const float& maxSpeed,
        float& speed, 
        glm::vec3& velocity, 
        glm::quat& rotation
    );
}
