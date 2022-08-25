#pragma once
#include "../../Types/Transform.h"
#include <bitset>
#include <algorithm>

#define DEFAULT_MIN_SPEED 10.0f
#define DEFAULT_MAX_SPEED 80.0f
#define DEFAULT_MIN_FRICTION 0.015f
#define DEFAULT_MAX_FRICTION 0.1f
#define DEFAULT_MOMENTUM_DECAY 0.15f
#define FRICTION_SPEED_CAP_PERCENT 0.35f
#define DEFAULT_ROTATION_SPEED 0.25f

#define SKI_ACCELERATION 3.0f
#define SKI_ROTATION_SPEED 0.02f
#define SKI_UPSLOPE_SCALING 1.5f
#define MINIMUM_SKI_ACCELERATION_SCALING 0.45f

enum RecieveMovementFrom {
    Player
};

enum MoveMode {
    Default,
    Ski,
    NumberOfModes
};

struct MovementCProperties {
    RecieveMovementFrom& recievesFrom;
    std::bitset<MoveMode::NumberOfModes>& supportedMoveModes;
    float& speed;
    float& minSpeed;
    float& maxSpeed;
    float& friction;
    float& minFriction;
    float& maxFriction;
    float& momentumDecay;
};

struct MovementComponent {
    vec3 desiredMovement[MAX_ENTITIES];
    MoveMode moveMode[MAX_ENTITIES];
    std::bitset<MoveMode::NumberOfModes> supportedMoveModes[MAX_ENTITIES];
    float speed[MAX_ENTITIES];
    float minSpeed[MAX_ENTITIES];
    float maxSpeed[MAX_ENTITIES];
    float friction[MAX_ENTITIES];
    float minFriction[MAX_ENTITIES];
    float maxFriction[MAX_ENTITIES];
    float momentumDecay[MAX_ENTITIES];
    RecieveMovementFrom recievesFrom[MAX_ENTITIES];

    MovementComponent() {
        std::fill_n(desiredMovement, MAX_ENTITIES, vec3(0.0f, 0.0f, 0.0f));
        std::fill_n(moveMode, MAX_ENTITIES, MoveMode::Default);
        std::fill_n(speed, MAX_ENTITIES, DEFAULT_MIN_SPEED);
        std::fill_n(minSpeed, MAX_ENTITIES, DEFAULT_MIN_SPEED);
        std::fill_n(maxSpeed, MAX_ENTITIES, DEFAULT_MAX_SPEED);
        std::fill_n(friction, MAX_ENTITIES, DEFAULT_MAX_FRICTION);
        std::fill_n(minFriction, MAX_ENTITIES, DEFAULT_MIN_FRICTION);
        std::fill_n(maxFriction, MAX_ENTITIES, DEFAULT_MAX_FRICTION);
        std::fill_n(momentumDecay, MAX_ENTITIES, DEFAULT_MOMENTUM_DECAY);
        std::fill_n(recievesFrom, MAX_ENTITIES, RecieveMovementFrom::Player);
    };
    MovementComponent(const MovementComponent&) = delete;
    MovementComponent& operator=(const MovementComponent&) = delete;

    MovementCProperties operator[](int index) {
        return MovementCProperties {
            recievesFrom[index],
            supportedMoveModes[index],
            speed[index],
            minSpeed[index],
            maxSpeed[index],
            friction[index],
            minFriction[index],
            maxFriction[index],
            momentumDecay[index]
        };
    }

    static int ID;
};