#pragma once
#include <glm/vec3.hpp>
#include <bitset>
#include "Component.h"

const float DEFAULT_MIN_SPEED = 12.0f;
const float DEFAULT_MAX_SPEED = 120.0f;
const float DEFAULT_MIN_FRICTION = 0.015f;
const float DEFAULT_MAX_FRICTION = 0.1f;
const float DEFAULT_MOMENTUM_DECAY = 0.1f;
const float FRICTION_SPEED_CAP_PERCENT = 0.35f;
const float DEFAULT_ROTATION_SPEED = 0.25f;
const float FLOW_ROTATION_SPEED = 0.06f;
const float LINE_ROTATION_SPEED = 0.025f;

const float SKI_ACCELERATION = 4.0f;
const float SKI_ROTATION_SPEED = 0.02f;
const float SKI_DOWNSLOPE_SCALING = 3.0f;
const float SKI_UPSLOPE_SCALING = 1.5f;
const float MINIMUM_SKI_ACCELERATION_SCALING = 0.15f;

enum MoveMode {
    Default,
    Ski,
    Flow,
    Line,
    Air,
    NumberOfModes
};

class MovementComponent : public Component {
public:
    std::array<glm::vec3, MAX_ENTITIES> desiredMovement;
    std::array<MoveMode, MAX_ENTITIES> moveMode;
    std::array<float, MAX_ENTITIES> speed;
    std::array<float, MAX_ENTITIES> minSpeed;
    std::array<float, MAX_ENTITIES> maxSpeed;
    std::array<float, MAX_ENTITIES> friction;
    std::array<float, MAX_ENTITIES> minFriction;
    std::array<float, MAX_ENTITIES> maxFriction;
    std::array<float, MAX_ENTITIES> momentumDecay;
    std::array<float, MAX_ENTITIES> tiltStrength;
    std::array<float, MAX_ENTITIES> tiltSpeed;

    MovementComponent() {
        desiredMovement.fill(glm::vec3(0.0f, 0.0f, 0.0f));
        moveMode.fill(MoveMode::Default);
    };
    MovementComponent(const MovementComponent&) = delete;
    MovementComponent& operator=(const MovementComponent&) = delete;

    static constexpr std::string GetName() { return "movement"; }
    static constexpr int GetID() { return 4; }

    void Load(nlohmann::json& data, EntityID entity) {
        minSpeed[entity] = GetFloat(data, "min_speed", DEFAULT_MIN_SPEED);
        maxSpeed[entity] = GetFloat(data, "max_speed", DEFAULT_MAX_SPEED);
        speed[entity] = minSpeed[entity];

        minFriction[entity] = GetFloat(data, "min_friction", DEFAULT_MIN_FRICTION);
        maxFriction[entity] = GetFloat(data, "max_friction", DEFAULT_MAX_FRICTION);
        friction[entity] = maxFriction[entity];

        momentumDecay[entity] = GetFloat(data, "momentum_decay", DEFAULT_MOMENTUM_DECAY);
        tiltStrength[entity] = GetFloat(data, "tilt_strength", 0.0f);
        tiltSpeed[entity] = GetFloat(data, "tilt_speed", 0.05f);
    }
};
