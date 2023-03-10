#pragma once
#include "../Entity/Entity.h"
#include "Component.h"
#include <glm.hpp>
#include <algorithm>

const float STEP_UP_HEIGHT = 0.25f;

enum StickType {
    None,
    StickOnly,
    StepUp 
};

struct GroundTraceCProperties {
    float& distance;
    StickType& stickType; 
};

struct GroundTraceComponent : public Component {
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

    GroundTraceCProperties operator[](int index) {
        return GroundTraceCProperties {
            distance[index],
            stickType[index]
        };
    }

    uint8_t GetID() const { return 1; }
    void Load(nlohmann::json& data, uint16_t entity) {
        distance[entity] = data["distance"].get<float>();
        stickType[entity]= (StickType)(uint8_t)data["stickType"].get<double>(); 
    }
};
