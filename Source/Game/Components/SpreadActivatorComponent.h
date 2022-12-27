#pragma once
#include "../Entity/Entity.h"
#include <algorithm>

const float NO_SPREAD = 0.0f;

struct SpreadActivatorCProperties {
    float& radius;
    bool& groundOnly;
};

struct SpreadActivatorComponent {
    float radius[MAX_ENTITIES];
    bool groundOnly[MAX_ENTITIES];

    SpreadActivatorComponent() {
        std::fill_n(radius, MAX_ENTITIES, NO_SPREAD);    
        std::fill_n(groundOnly, MAX_ENTITIES, true);
    };
    SpreadActivatorComponent(const SpreadActivatorComponent&) = delete;
    SpreadActivatorComponent& operator=(const SpreadActivatorComponent&) = delete;

    SpreadActivatorCProperties operator[](int index) {
        return {
            radius[index],
            groundOnly[index]
        };
    };

    static int ID;
};
