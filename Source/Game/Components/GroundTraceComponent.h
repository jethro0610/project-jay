#pragma once
#include "Component.h"
#include <glm/vec3.hpp>

const float STEP_UP_HEIGHT = 0.25f;

class GroundTraceComponent : public Component {
public:
    std::array<float, MAX_ENTITIES> distance;
    std::array<bool, MAX_ENTITIES> stick;
    std::array<float, MAX_ENTITIES> stickOffset;
    std::array<bool, MAX_ENTITIES> zeroVelocity;
    std::array<float, MAX_ENTITIES> alignStrength;

    std::array<bool, MAX_ENTITIES> onGround;
    std::array<bool, MAX_ENTITIES> onGroundLastFrame;
    std::array<bool, MAX_ENTITIES> enteredGround;
    std::array<bool, MAX_ENTITIES> exitedGround;
    std::array<float, MAX_ENTITIES> groundPosition;
    std::array<glm::vec3, MAX_ENTITIES> groundNormal;

    std::array<bool, MAX_ENTITIES> destroyOnGround;

    GroundTraceComponent() {
        onGround.fill(false);
        enteredGround.fill(false);
        exitedGround.fill(false);
        onGroundLastFrame.fill(false);
        groundPosition.fill(0.0f);
        groundNormal.fill(glm::vec3(0.0f, 0.0f, 0.0f));
        destroyOnGround.fill(false);
    };
    GroundTraceComponent(const GroundTraceComponent&) = delete;
    GroundTraceComponent& operator=(const GroundTraceComponent&) = delete;

    static constexpr std::string GetName() { return "ground_trace"; }
    static constexpr int GetID() { return 1; }

    void Load(nlohmann::json& data, EntityID entity) {
        distance[entity] = GetFloat(data, "distance", 1.0f);
        stick[entity] = GetBoolean(data, "stick", false);
        stickOffset[entity] = GetFloat(data, "stick_offset", 0.0f);
        zeroVelocity[entity] = GetBoolean(data, "zero_velocity", false);
        alignStrength[entity] = GetFloat(data, "align_strength", 0.0f);
        destroyOnGround[entity] = GetBoolean(data, "destroy_on_ground", false);
    }
};
