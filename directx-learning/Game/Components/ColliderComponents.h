#pragma once
#include "EntityConstants.h"
#include "../../Types/Transform.h"

struct ColliderCProperties {
    float& radius;
};

struct ColliderComponents {
    float radius[MAX_ENTITIES];
    vec3 hitNormal[MAX_ENTITIES];
    bool hit[MAX_ENTITIES];
    ColliderComponents() {
        std::fill_n(radius, MAX_ENTITIES, -1.0f);
        std::fill_n(hitNormal, MAX_ENTITIES, vec3(0.0f, 0.0f, 0.0f));
        std::fill_n(hit, MAX_ENTITIES, false);
    };
    ColliderComponents(const ColliderComponents&) = delete;
    ColliderComponents& operator=(const ColliderComponents&) = delete;

    ColliderCProperties operator[](int index) {
        return ColliderCProperties{
            radius[index]
        };
    }
};