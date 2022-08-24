#pragma once
#include "../Entity/Entity.h"
#include "../../Types/Transform.h"
#include <algorithm>

struct VelocityCProperties {
    vec3& velocity;
};

struct VelocityComponent {
    vec3 velocity[MAX_ENTITIES];

    VelocityComponent() {
        std::fill_n(velocity, MAX_ENTITIES, vec3(0.0f, 0.0f, 0.0f));
    };
    VelocityComponent(const VelocityComponent&) = delete;
    VelocityComponent& operator=(const VelocityComponent&) = delete;

    VelocityCProperties operator[](int index) {
        return VelocityCProperties{
            velocity[index]
        };
    }

    static int ID;
};