#pragma once
#include "Component.h"

enum ProjectileType {
    Random,
    RiseAndFall,
    Target,
    NumOfProjectileTypes
};

class ProjectileComponent : public Component {
public:
    ProjectileType type[MAX_ENTITIES];
    EntityID target[MAX_ENTITIES];
    float param1[MAX_ENTITIES];
    float param2[MAX_ENTITIES];

    ProjectileComponent() {
        std::fill_n(type, MAX_ENTITIES, NumOfProjectileTypes);
        std::fill_n(target, MAX_ENTITIES, NULL_ENTITY);
        std::fill_n(param1, MAX_ENTITIES, 0.0f);
        std::fill_n(param2, MAX_ENTITIES, 0.0f);
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

        param1[entity] = data["param1"].get<float>();
        param2[entity] = data["param2"].get<float>();
    }
};
