#pragma once
#include <bitset>
#include "Component.h"

union ProjParam1 {
    float speed;
    float trackingStrength;
};
union ProjParam2 {
    float height;
};
enum ProjectileType {
    Random,
    RiseAndFall,
    Target,
    NumOfProjectileTypes
};
enum ContactBehavior {
    SelfDamage,
    BounceBack,
    Deactivate,
    NumContactBehaviors
};

class ProjectileComponent : public Component {
public:
    std::array<ProjectileType, MAX_ENTITIES> type;
    std::array<int, MAX_ENTITIES> damage;
    std::array<ProjParam1, MAX_ENTITIES> param1;
    std::array<ProjParam2, MAX_ENTITIES> param2;
    std::array<std::bitset<NumContactBehaviors>, MAX_ENTITIES> contactBehavior; 

    std::array<EntityID, MAX_ENTITIES> target;
    std::array<bool, MAX_ENTITIES> active;

    ProjectileComponent() {
        target.fill(NULL_ENTITY);
        active.fill(false);
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

        damage[entity] = GetFloat(data, "damage", 0.0f);

        param1[entity].speed = GetFloat(data, "speed", 0.0f);
        param1[entity].trackingStrength = GetFloat(data, "tracking_strength", param1[entity].speed);
        param2[entity].height = GetFloat(data, "height", param2[entity].height);

        contactBehavior[entity].set(SelfDamage, GetBoolean(data, "contact_self_damage"));
        contactBehavior[entity].set(BounceBack, GetBoolean(data, "contact_bounce_back"));
        contactBehavior[entity].set(Deactivate, GetBoolean(data, "contact_deactivate"));
    }
};
