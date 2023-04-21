#pragma once
#include "Component.h"

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

class ProjectileComponent : public Component {
public:
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

    static constexpr std::string GetName() { return "projectile"; }
    static constexpr uint8_t GetID() { return 6; }

    void Load(nlohmann::json& data, EntityID entity) {
        std::string projectileTypeS = data["type"].get<std::string>();
        if (projectileTypeS == "random")
            type[entity] = Random;
        else if (projectileTypeS == "rise_and_fall")
            type[entity] = RiseAndFall;
        param1[entity] = data["param1"].get<float>();
        param2[entity] = data["param2"].get<float>();
    }
};
