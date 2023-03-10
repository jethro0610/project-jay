#pragma once
#include "../Entity/Entity.h"
#include "Component.h"
#include <glm.hpp>
#include <algorithm>

struct VelocityCProperties {
    glm::vec3& velocity;
};

struct VelocityComponent : public Component {
    glm::vec3 velocity[MAX_ENTITIES];

    VelocityComponent() {
        std::fill_n(velocity, MAX_ENTITIES, glm::vec3(0.0f, 0.0f, 0.0f));
    };
    VelocityComponent(const VelocityComponent&) = delete;
    VelocityComponent& operator=(const VelocityComponent&) = delete;

    VelocityCProperties operator[](int index) {
        return VelocityCProperties{
            velocity[index]
        };
    }

    inline static int ID = 14;
    void Load(nlohmann::json& data, uint16_t entity) {
    }
};
