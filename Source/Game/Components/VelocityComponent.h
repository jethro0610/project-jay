#pragma once
#include <glm/vec3.hpp>
#include "Component.h"
#include "../../Helpers/LoadHelpers.h"

const float GRAVITY_ACCELERATION = 1.0f;
const float MAX_GRAVITY = 60.0f;

class VelocityComponent : public Component {
public:
    glm::vec3 velocity[MAX_ENTITIES];
    bool useGravity[MAX_ENTITIES];

    VelocityComponent() {
        std::fill_n(velocity, MAX_ENTITIES, glm::vec3(0.0f, 0.0f, 0.0f));
        std::fill_n(useGravity, MAX_ENTITIES, false);
    };
    VelocityComponent(const VelocityComponent&) = delete;
    VelocityComponent& operator=(const VelocityComponent&) = delete;

    static constexpr std::string GetName() { return "velocity"; }
    static constexpr int GetID() { return 11; }

    void Load(nlohmann::json& data, EntityID entity) {
        useGravity[entity] = GetBoolean(data, "use_gravity");
    }
};
