#pragma once
#include <glm/vec3.hpp>
#include "Component.h"

class VelocityComponent : public Component {
public:
    glm::vec3 velocity[MAX_ENTITIES];

    VelocityComponent() {
        std::fill_n(velocity, MAX_ENTITIES, glm::vec3(0.0f, 0.0f, 0.0f));
    };
    VelocityComponent(const VelocityComponent&) = delete;
    VelocityComponent& operator=(const VelocityComponent&) = delete;

    static constexpr std::string GetName() { return "velocity"; }
    static constexpr uint8_t GetID() { return 11; }

    void Load(nlohmann::json& data, EntityID entity) {
    }
};
