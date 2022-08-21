#pragma once
#include "EntityConstants.h"

#define NO_HIT_NORMAL vec3(0.0f, 0.0f, 0.0f)

struct ColliderComponents {
    float radius[MAX_ENTITIES];
    vec3 hitNormal[MAX_ENTITIES];

    ColliderComponents() {
        std::fill_n(radius, MAX_ENTITIES, -1.0f);
        std::fill_n(hitNormal, MAX_ENTITIES, NO_HIT_NORMAL);
    };
    ColliderComponents(const ColliderComponents&) = delete;
    ColliderComponents& operator=(const ColliderComponents&) = delete;
};