#pragma once
#include "../Entity/Entity.h"
#include <algorithm>

struct SpreadActivatorCProperties {
    bool& groundOnly;
};

struct SpreadActivatorComponent {
    bool groundOnly[MAX_ENTITIES];
    bool active[MAX_ENTITIES];

    SpreadActivatorComponent() {
        std::fill_n(groundOnly, MAX_ENTITIES, true);
        std::fill_n(active, MAX_ENTITIES, false);
    };
    SpreadActivatorComponent(const SpreadActivatorComponent&) = delete;
    SpreadActivatorComponent& operator=(const SpreadActivatorComponent&) = delete;

    SpreadActivatorCProperties operator[](int index) {
        return {
            groundOnly[index]
        };
    };

    static int ID;
};
