#pragma once
#include <array>
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include "../Entity/EntityLimits.h"
class Entity;
class GroundTraceComponent;
class MovementComponent;
class SpreadDetectComponent;
class TransformComponent;
class VelocityComponent;

namespace MovementSystem {
    void Execute(
        std::array<Entity, MAX_ENTITIES>& entities,
        GroundTraceComponent& groundTraceComponent,
        MovementComponent& movementComponent,
        SpreadDetectComponent& spreadDetectComponent,
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent
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

    void CalculateFlowMovement(
        const glm::vec3& desiredMovement,
        const float& speed,
        glm::vec3& velocity,
        glm::quat& rotation
    );

    void CalculateLineMovement(
        const glm::vec3& desiredMovement,
        const float& speed,
        glm::vec3& velocity,
        glm::quat& rotation
    );
}
