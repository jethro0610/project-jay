#pragma once
#include "Component.h"
#include "Game/Collision/Collider.h"

struct Hurtbox : public Collider {
    bool recieveKnockback;
    bool rotate;
};

class HurtboxComponent: public Component {
public:
    std::array<Hurtbox, MAX_ENTITIES> hurtbox;
    std::array<int, MAX_ENTITIES> cooldown;
    std::array<bool, MAX_ENTITIES> hurt;

    HurtboxComponent() {

    };
    HurtboxComponent(const HurtboxComponent&) = delete;
    HurtboxComponent& operator=(const HurtboxComponent&) = delete;

    static constexpr std::string GetName() { return "hurtbox"; }
    static constexpr int GetID() { return 17; }

    void Load(nlohmann::json& data, EntityID entity) {
        hurtbox[entity].radius = GetFloat(data, "radius");
        hurtbox[entity].top = GetFloat(data, "top");
        hurtbox[entity].bottom = GetFloat(data, "bottom");
        hurtbox[entity].recieveKnockback = GetBoolean(data, "recieve_knockback", true);
        hurtbox[entity].rotate = GetBoolean(data, "rotate");
        cooldown[entity] = 0;
        hurt[entity] = false;
    }
};
