#pragma once
#include "../Entity/Entity.h"
#include <algorithm>

#include "ComponentDesc.h"

struct WorldColliderCProperties {
};

struct WorldColliderComponent {
    // TODO: Add radius
    bool colliding[MAX_ENTITIES];

    WorldColliderComponent() {
    };
    WorldColliderComponent(const WorldColliderComponent&) = delete;
    WorldColliderComponent& operator=(const WorldColliderComponent&) = delete;

    WorldColliderCProperties operator[](int index) {
        return WorldColliderCProperties {
        };
    }

    inline static int ID = 15;
    inline static ComponentDesc description {
        "World Collider",
        0,
        {}
    };
};
