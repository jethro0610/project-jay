#pragma once
#include "../Entity/Entity.h"
#include <algorithm>

enum ProjectileType {
    Random,
    RiseAndFall
};

enum ProjectileState {
    Inactive,
    Launch,
    Rise,
    Fall,
    Throw
};

struct ProjectileCProperties {
    ProjectileType& type;
    float& param1;
    float& param2;
};

struct ProjectileComponent {
    ProjectileType type[MAX_ENTITIES];
    ProjectileState state[MAX_ENTITIES];
    float param1[MAX_ENTITIES];
    float param2[MAX_ENTITIES];

    ProjectileComponent () {
        std::fill_n(type, MAX_ENTITIES, Random);
        std::fill_n(state, MAX_ENTITIES, Inactive);
        std::fill_n(param1, MAX_ENTITIES, 0.0f);
        std::fill_n(param2, MAX_ENTITIES, 0.0f);
    }

    ProjectileCProperties operator[](int index) {
        return {
            type[index],
            param1[index],
            param2[index]
        };
    }
    static int ID;
};
