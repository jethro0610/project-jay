#pragma once
#include <glm/vec3.hpp>
#include "Component.h"
#include "../../Helpers/LoadHelpers.h"

const float STEP_UP_HEIGHT = 0.25f;

class GroundTraceComponent : public Component {
public:
    float distance[MAX_ENTITIES];
    bool stick[MAX_ENTITIES];
    float stickOffset[MAX_ENTITIES];
    bool forceNoGroundThisFrame[MAX_ENTITIES];

    bool onGround[MAX_ENTITIES];
    bool onGroundLastFrame[MAX_ENTITIES];
    bool enteredGround[MAX_ENTITIES];
    bool exitedGround[MAX_ENTITIES];
    float groundPosition[MAX_ENTITIES];
    glm::vec3 groundNormal[MAX_ENTITIES];

    GroundTraceComponent() {
        std::fill_n(distance, MAX_ENTITIES, 0.0f);
        std::fill_n(stick, MAX_ENTITIES, false);
        std::fill_n(stickOffset, MAX_ENTITIES, 0.0f);
        std::fill_n(forceNoGroundThisFrame, MAX_ENTITIES, false);

        std::fill_n(onGround, MAX_ENTITIES, false);
        std::fill_n(enteredGround, MAX_ENTITIES, false);
        std::fill_n(exitedGround, MAX_ENTITIES, false);
        std::fill_n(onGroundLastFrame, MAX_ENTITIES, false);
        std::fill_n(groundPosition, MAX_ENTITIES, 0.0f);
        std::fill_n(groundNormal, MAX_ENTITIES, glm::vec3(0.0f, 0.0f, 0.0f));
    };
    GroundTraceComponent(const GroundTraceComponent&) = delete;
    GroundTraceComponent& operator=(const GroundTraceComponent&) = delete;

    static constexpr std::string GetName() { return "ground_trace"; }
    static constexpr int GetID() { return 1; }

    void Load(nlohmann::json& data, EntityID entity) {
        distance[entity] = data["distance"].get<float>();
        stick[entity] = GetBoolean(data, "stick", false);
        stickOffset[entity] = GetFloat(data, "stick_offset", 0.0f);
    }
};
