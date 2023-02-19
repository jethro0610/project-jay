#pragma once
#include "../Entity/Entity.h"
#include <algorithm>

struct ProjectileCProperties {
};

struct ProjectileComponent {
    bool active[MAX_ENTITIES];

    ProjectileComponent () {
        std::fill_n(active, MAX_ENTITIES, false);
    }

    ProjectileCProperties operator[](int index) {
        return {
        };
    }
    static int ID;
};
