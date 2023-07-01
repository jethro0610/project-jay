#pragma once
#include <glm/vec3.hpp>
#include "Component.h"

const float STEP_UP_HEIGHT = 0.25f;

enum StickType {
    None,
    StickOnly,
    StepUp 
};

class GroundTraceComponent : public Component {
public:
    float distance[MAX_ENTITIES];
    StickType stickType[MAX_ENTITIES];
    bool onGround[MAX_ENTITIES];
    bool onGroundLastFrame[MAX_ENTITIES];
    bool enteredGround[MAX_ENTITIES];
    bool exitedGround[MAX_ENTITIES];
    float groundPosition[MAX_ENTITIES];
    glm::vec3 groundNormal[MAX_ENTITIES];

    GroundTraceComponent() {
        std::fill_n(distance, MAX_ENTITIES, 0.0f);
        std::fill_n(stickType, MAX_ENTITIES, StickType::None);
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
        std::string stickTypeS = data["type"].get<std::string>();
        if (stickTypeS == "no_stick")
            stickType[entity] = None;
        else if (stickTypeS == "stick_only")
            stickType[entity] = StickOnly;
        else if (stickTypeS == "step_up")
            stickType[entity] = StepUp;
    }
};
