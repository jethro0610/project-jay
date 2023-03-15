#pragma once
#include "Component.h"
#include <glm.hpp>
#include <algorithm>

struct VelocityComponent : public Component {
    glm::vec3 velocity[MAX_ENTITIES];

    VelocityComponent() {
        std::fill_n(velocity, MAX_ENTITIES, glm::vec3(0.0f, 0.0f, 0.0f));
    };
    VelocityComponent(const VelocityComponent&) = delete;
    VelocityComponent& operator=(const VelocityComponent&) = delete;

    std::string GetName() const { return "velocity"; }
    void Load(nlohmann::json& data, EntityID entity) {
    }
};
