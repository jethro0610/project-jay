#pragma once
#include "../Entity/Entity.h"
#include "Component.h"
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

struct ProjectileComponent : public Component {
    ProjectileType type[MAX_ENTITIES];
    ProjectileState state[MAX_ENTITIES];
    float param1[MAX_ENTITIES];
    float param2[MAX_ENTITIES];

    ProjectileComponent() {
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
    inline static int ID = 8;
    void Load(nlohmann::json& data, uint16_t entity) {
        type[entity] = (ProjectileType)(uint8_t)data["type"].get<double>();
        param1[entity] = data["param1"].get<float>();
        param2[entity] = data["param2"].get<float>();
    }
};
