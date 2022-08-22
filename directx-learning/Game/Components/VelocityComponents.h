#pragma once
#include "EntityConstants.h"
#include "../../Types/Transform.h"
#include <algorithm>

struct VelocityComponents {
    vec3 velocity[MAX_ENTITIES];

    VelocityComponents() {
        std::fill_n(velocity, MAX_ENTITIES, vec3(0.0f, 0.0f, 0.0f));
    };
    VelocityComponents(const VelocityComponents&) = delete;
    VelocityComponents& operator=(const VelocityComponents&) = delete;
};