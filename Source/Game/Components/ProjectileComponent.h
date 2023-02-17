#pragma once
#include "../Entity/Entity.h"
#include <algorithm>

struct ProjectileCProperties {
    float& radius;
};

struct ProjectileComponent {
    // Should probably move this to hitbox component instead
    float radius[MAX_ENTITIES];
    bool active[MAX_ENTITIES];

    ProjectileComponent () {
        std::fill_n(radius, MAX_ENTITIES, 1.0f);
        std::fill_n(active, MAX_ENTITIES, false);
    }

    ProjectileCProperties operator[](int index) {
        return {
            radius[index]
        };
    }
    static int ID;
};
