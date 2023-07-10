#pragma once
#include "Component.h"

enum ProjectileType {
    Random,
    RiseAndFall,
    Target,
    NumOfProjectileTypes
};

union ProjParam1 {
    float speed;
    float trackingStrength;
};

union ProjParam2 {
    float height;
};

class ProjectileComponent : public Component {
public:
    ProjectileType type[MAX_ENTITIES];
    EntityID target[MAX_ENTITIES];
    ProjParam1 param1[MAX_ENTITIES];
    ProjParam2 param2[MAX_ENTITIES];
    bool active[MAX_ENTITIES];

    ProjectileComponent() {
        std::fill_n(target, MAX_ENTITIES, NULL_ENTITY);
        std::fill_n(active, MAX_ENTITIES, false);
    }

    static constexpr std::string GetName() { return "projectile"; }
    static constexpr int GetID() { return 6; }

    void Load(nlohmann::json& data, EntityID entity) {
        std::string projectileTypeS = data["type"].get<std::string>();

        if (projectileTypeS == "random")
            type[entity] = Random;
        else if (projectileTypeS == "rise_and_fall")
            type[entity] = RiseAndFall;
        else if (projectileTypeS == "target")
            type[entity] = Target;

        param1[entity].speed = GetFloat(data, "speed", 0.0f);
        param1[entity].trackingStrength = GetFloat(data, "tracking_strength", param1[entity].speed);

        param2[entity].height = GetFloat(data, "height", param2[entity].height);
    }
};
