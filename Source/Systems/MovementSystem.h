#pragma once
#include "Systems/SystemInc.h"
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>

namespace MovementSystem {
    void Execute(
        EntityList& entities,
        ComponentList& components
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
