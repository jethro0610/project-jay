#pragma once
#include "Component.h"
#include "Collider.h"

struct Hitbox : public Collider {
    float horizontalKb;
    float verticalKb;
    int damage;
    bool active;
};

class HitboxComponent: public Component {
public:
    std::array<Hitbox, MAX_ENTITIES> hitbox;

    HitboxComponent() {

    };
    HitboxComponent(const HitboxComponent&) = delete;
    HitboxComponent& operator=(const HitboxComponent&) = delete;

    static constexpr std::string GetName() { return "hitbox"; }
    static constexpr int GetID() { return 16; }

    void Load(nlohmann::json& data, EntityID entity) {
        hitbox[entity].radius = GetFloat(data, "radius");
        hitbox[entity].height = GetFloat(data, "height");
        hitbox[entity].horizontalKb = GetFloat(data, "horizontal_kb");
        hitbox[entity].verticalKb = GetFloat(data, "vertical_kb");
        hitbox[entity].damage = GetInt(data, "damage");
    }
};
