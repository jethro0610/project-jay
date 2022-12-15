#pragma once
#include "../Entity/Entity.h"
#include "../../Types/Transform.h"
#include <algorithm>

struct WorldColliderCProperties {
};

struct WorldColliderComponent {
    bool colliding[MAX_ENTITIES];

    WorldColliderComponent() {
    };
    WorldColliderComponent(const WorldColliderComponent&) = delete;
    WorldColliderComponent& operator=(const WorldColliderComponent&) = delete;

    WorldColliderCProperties operator[](int index) {
        return WorldColliderCProperties {
        };
    }

    static int ID;
};
