#pragma once
#include "../Entity/Entity.h"
#include "Component.h"
#include <glm.hpp>
#include <bitset>
#include <algorithm>

const float DEFAULT_MIN_SPEED = 12.0f;
const float DEFAULT_MAX_SPEED = 120.0f;
const float DEFAULT_MIN_FRICTION = 0.015f;
const float DEFAULT_MAX_FRICTION = 0.1f;
const float DEFAULT_MOMENTUM_DECAY = 0.1f;
const float FRICTION_SPEED_CAP_PERCENT = 0.35f;
const float DEFAULT_ROTATION_SPEED = 0.25f;
const float FLOW_ROTATION_SPEED = 0.06f;

const float SKI_ACCELERATION = 4.0f;
const float SKI_ROTATION_SPEED = 0.02f;
const float SKI_UPSLOPE_SCALING = 1.5f;
const float MINIMUM_SKI_ACCELERATION_SCALING = 0.15f;

enum MoveMode {
    Default,
    Ski,
    Flow,
    NumberOfModes
};

struct MovementCProperties {
    float& speed;
    float& minSpeed;
    float& maxSpeed;
    float& friction;
    float& minFriction;
    float& maxFriction;
    float& momentumDecay;
};

struct MovementComponent : public Component {
    glm::vec3 desiredMovement[MAX_ENTITIES];
    MoveMode moveMode[MAX_ENTITIES];
    float speed[MAX_ENTITIES];
    float minSpeed[MAX_ENTITIES];
    float maxSpeed[MAX_ENTITIES];
    float friction[MAX_ENTITIES];
    float minFriction[MAX_ENTITIES];
    float maxFriction[MAX_ENTITIES];
    float momentumDecay[MAX_ENTITIES];

    MovementComponent() {
        std::fill_n(desiredMovement, MAX_ENTITIES, glm::vec3(0.0f, 0.0f, 0.0f));
        std::fill_n(moveMode, MAX_ENTITIES, MoveMode::Default);
        std::fill_n(speed, MAX_ENTITIES, DEFAULT_MIN_SPEED);
        std::fill_n(minSpeed, MAX_ENTITIES, DEFAULT_MIN_SPEED);
        std::fill_n(maxSpeed, MAX_ENTITIES, DEFAULT_MAX_SPEED);
        std::fill_n(friction, MAX_ENTITIES, DEFAULT_MAX_FRICTION);
        std::fill_n(minFriction, MAX_ENTITIES, DEFAULT_MIN_FRICTION);
        std::fill_n(maxFriction, MAX_ENTITIES, DEFAULT_MAX_FRICTION);
        std::fill_n(momentumDecay, MAX_ENTITIES, DEFAULT_MOMENTUM_DECAY);
    };
    MovementComponent(const MovementComponent&) = delete;
    MovementComponent& operator=(const MovementComponent&) = delete;

    MovementCProperties operator[](int index) {
        return MovementCProperties {
            speed[index],
            minSpeed[index],
            maxSpeed[index],
            friction[index],
            minFriction[index],
            maxFriction[index],
            momentumDecay[index]
        };
    }

    inline static int ID = 6;
    void Load(nlohmann::json& data, uint8_t index, uint16_t entity) {
        minSpeed[entity] = data["components"][index]["minSpeed"].get<float>();
        maxSpeed[entity] = data["components"][index]["maxSpeed"].get<float>();
        minFriction[entity] = data["components"][index]["minFriction"].get<float>();
        maxFriction[entity] = data["components"][index]["maxFriction"].get<float>();
        momentumDecay[entity] = data["components"][index]["momentumDecay"].get<float>();
    }
};
