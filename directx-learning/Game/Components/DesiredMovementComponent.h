#pragma once
#include "../../Types/Transform.h"
#include <bitset>
#include <algorithm>

#define DEFAULT_MAX_SPEED 15.0f
#define DEFAULT_FRICTION 0.1f

enum RecieveMovementFrom {
    Player
};

enum MoveMode {
    Default,
    Ski,

    NumberOfModes = Ski
};

struct DesiredMovementCProperties {
    RecieveMovementFrom& recievesFrom;
    std::bitset<MoveMode::NumberOfModes + 1>& supportedMoveModes;
    float& maxSpeed;
    float& friction;
};

struct DesiredMovementComponent {
    vec3 desiredMovement[MAX_ENTITIES];
    MoveMode moveMode[MAX_ENTITIES];
    std::bitset<MoveMode::NumberOfModes + 1> supportedMoveModes[MAX_ENTITIES];
    float maxSpeed[MAX_ENTITIES];
    float friction[MAX_ENTITIES];
    RecieveMovementFrom recievesFrom[MAX_ENTITIES];

    DesiredMovementComponent() {
        std::fill_n(desiredMovement, MAX_ENTITIES, vec3(0.0f, 0.0f, 0.0f));
        std::fill_n(moveMode, MAX_ENTITIES, MoveMode::Default);
        std::fill_n(maxSpeed, MAX_ENTITIES, DEFAULT_MAX_SPEED);
        std::fill_n(friction, MAX_ENTITIES, DEFAULT_FRICTION);
        std::fill_n(recievesFrom, MAX_ENTITIES, RecieveMovementFrom::Player);
    };
    DesiredMovementComponent(const DesiredMovementComponent&) = delete;
    DesiredMovementComponent& operator=(const DesiredMovementComponent&) = delete;

    DesiredMovementCProperties operator[](int index) {
        return DesiredMovementCProperties{
            recievesFrom[index],
            supportedMoveModes[index],
            maxSpeed[index],
            friction[index]
        };
    }

    static int ID;
};