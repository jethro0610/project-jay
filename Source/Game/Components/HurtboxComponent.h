#pragma once
#include "Component.h"
#include "../Collision/Collider.h"

struct Hurtbox : public Collider {
    int cooldown;
};

class HurtboxComponent: public Component {
public:
    std::array<Hurtbox, MAX_ENTITIES> hurtbox;

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
        hurtbox[entity].cooldown = 0;
    }
};
