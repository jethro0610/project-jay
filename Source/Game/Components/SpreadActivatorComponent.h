#pragma once
#include "../Entity/Entity.h"
#include <algorithm>

struct SpreadActivatorCProperties {
    float& radius;
    bool& groundOnly;
};

struct SpreadActivatorComponent {
    float radius[MAX_ENTITIES];
    bool groundOnly[MAX_ENTITIES];

    SpreadActivatorComponent() {
        std::fill_n(radius, MAX_ENTITIES, 0.0f);    
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
