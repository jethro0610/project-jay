#pragma once
#include <glm.hpp>
class EntityManager;

const float GRAVITY_ACCELERATION = 1.0f;
const float MAX_GRAVITY = 60.0f;

namespace MovementSystem {
    void Execute(EntityManager& entityManager);

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
        const glm::vec3& groundNormal,
        const float& speed,
        glm::vec3& velocity,
        glm::quat& rotation
    );

    void CalculateLineMovement(
        const glm::vec3& desiredMovement,
        const glm::vec3& groundNormal,
        const float& speed,
        glm::vec3& velocity,
        glm::quat& rotation
    );
}
