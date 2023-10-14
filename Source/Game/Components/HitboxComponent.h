#pragma once
#include "Component.h"
#include "../Collision/Collider.h"

struct Hitbox : public Collider {
    bool useVelocity;
    float directionInfluence;
    float horizontalKb;
    float verticalKb;

    int damage;
    bool active;
};

class HitboxComponent: public Component {
public:
    std::array<Hitbox, MAX_ENTITIES> hitbox;
    std::array<int, MAX_ENTITIES> cooldown;

    HitboxComponent() {

    };
    HitboxComponent(const HitboxComponent&) = delete;
    HitboxComponent& operator=(const HitboxComponent&) = delete;

    static constexpr std::string GetName() { return "hitbox"; }
    static constexpr int GetID() { return 16; }

    void Load(nlohmann::json& data, EntityID entity) {
        hitbox[entity].radius = GetFloat(data, "radius");
        hitbox[entity].top = GetFloat(data, "top");
        hitbox[entity].bottom = GetFloat(data, "bottom");
        hitbox[entity].horizontalKb = GetFloat(data, "horizontal_kb", GetFloat(data, "horizontal_vel_mult", 1.0f));
        hitbox[entity].verticalKb = GetFloat(data, "vertical_kb", 1.0f);
        hitbox[entity].damage = GetInt(data, "damage");
        hitbox[entity].active = GetBoolean(data, "active", false);
        hitbox[entity].useVelocity = GetBoolean(data, "use_velocity", false);
        hitbox[entity].directionInfluence = GetFloat(data, "direction_influence", false);
        cooldown[entity] = 0;
    }
};
