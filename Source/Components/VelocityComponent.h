#pragma once
#include "Component.h"
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

class VelocityComponent : public Component {
public:
    static constexpr float GRAVITY_ACCELERATION = 1.0f;
    static constexpr float MAX_GRAVITY = 120.0f;

    std::array<glm::vec3, MAX_ENTITIES> velocity;
    std::array<glm::quat, MAX_ENTITIES> angularVelocity;
    std::array<bool, MAX_ENTITIES> useGravity;

    VelocityComponent() {
        velocity.fill(glm::vec3(0.0f, 0.0f, 0.0f));
        angularVelocity.fill(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
        useGravity.fill(false);
    };
    VelocityComponent(const VelocityComponent&) = delete;
    VelocityComponent& operator=(const VelocityComponent&) = delete;

    static constexpr std::string GetName() { return "velocity"; }
    static constexpr int GetID() { return 11; }

    void Load(nlohmann::json& data, EntityID entity) {
        useGravity[entity] = GetBoolean(data, "use_gravity");
    }
};
