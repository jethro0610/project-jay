#pragma once
#include "EntityConstants.h"
#include "../../Types/Transform.h"
#include <algorithm>

#define RECIEVE_MOVEMENT_NONE -1
#define RECIEVE_MOVEMENT_PLAYER 0

struct DesiredMovementComponents {
    vec3 desiredMovement[MAX_ENTITIES];
    int recievesFrom[MAX_ENTITIES];

    DesiredMovementComponents() {
        std::fill_n(desiredMovement, MAX_ENTITIES, vec3(0.0f, 0.0f, 0.0f));
        std::fill_n(recievesFrom, MAX_ENTITIES, RECIEVE_MOVEMENT_NONE);
    };
    DesiredMovementComponents(const DesiredMovementComponents&) = delete;
    DesiredMovementComponents& operator=(const DesiredMovementComponents&) = delete;
};