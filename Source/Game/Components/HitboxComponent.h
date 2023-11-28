#pragma once
#include "Component.h"
#include "../../Logging/Logger.h"
#include <vector_const.h>
#include "../Collision/Collider.h"

const int MAX_HITBOXES = 8;

struct Hitbox : public Collider {
    bool useVelocity;
    float directionInfluence;
    float horizontalKb;
    float verticalKb;
    float minHorizontalKb;

    int hitlag;
    int damage;
    bool active;
};

class HitboxComponent: public Component {
public:
    std::array<vector_const<Hitbox, MAX_HITBOXES>, MAX_ENTITIES> hitboxes;
    std::array<bool, MAX_ENTITIES> hit;

    HitboxComponent() {

    };
    HitboxComponent(const HitboxComponent&) = delete;
    HitboxComponent& operator=(const HitboxComponent&) = delete;

    static constexpr std::string GetName() { return "hitbox"; }
    static constexpr int GetID() { return 16; }

    void Load(nlohmann::json& data, EntityID entity) {
        hitboxes[entity].resize(0);
        hit[entity] = false;

        if (!data.contains("hitboxes"))
            return;

        auto hitboxesData = data["hitboxes"];
        for (auto& hitboxData : hitboxesData) {
            Hitbox hitbox;
            hitbox.radius = GetFloat(hitboxData, "radius");
            hitbox.top = GetFloat(hitboxData, "top");
            hitbox.bottom = GetFloat(hitboxData, "bottom");
            hitbox.horizontalKb = GetFloat(hitboxData, "horizontal_kb", GetFloat(hitboxData, "horizontal_vel_mult", 1.0f));
            hitbox.verticalKb = GetFloat(hitboxData, "vertical_kb", 1.0f);
            hitbox.minHorizontalKb = GetFloat(hitboxData, "min_horizontal_kb", 0.0f);
            hitbox.damage = GetInt(hitboxData, "damage");
            hitbox.active = GetBoolean(hitboxData, "active", false);
            hitbox.useVelocity = GetBoolean(hitboxData, "use_velocity", false);
            hitbox.directionInfluence = GetFloat(hitboxData, "direction_influence", false);
            hitbox.hitlag = GetInt(hitboxData, "hitlag", 3);
            hitboxes[entity].push_back(hitbox);
        }
    }
};
