#pragma once
#include "Component.h"
#include "Collision/Collider.h"

struct Hurtbox : public Collider {
    bool recieveKnockback;
    bool rotate;
};

class HurtboxComponent: public Component {
public:
    enum FaceDirection {
        FD_None,
        FD_Forward,
        FD_Backward
    };
    std::array<Hurtbox, MAX_ENTITIES> hurtbox;
    std::array<int, MAX_ENTITIES> cooldown;
    std::array<bool, MAX_ENTITIES> hurt;

    std::array<bool, MAX_ENTITIES> stun;
    std::array<int, MAX_ENTITIES> stunTimer;
    std::array<int, MAX_ENTITIES> stunTime;
    std::array<bool, MAX_ENTITIES> endStunOnGround;

    std::array<int, MAX_ENTITIES> seedAmount;
    std::array<float, MAX_ENTITIES> seedRadius;

    std::array<FaceDirection, MAX_ENTITIES> faceDirection;

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
        stunTime[entity] = GetBoolean(data, "stun_time", 0);
        endStunOnGround[entity] = GetBoolean(data, "end_stun_on_ground", false);
        seedAmount[entity] = GetInt(data, "seed", 0);
        seedRadius[entity] = GetFloat(data, "seed_radius", 0);

        std::string directionString = GetString(data, "face_direction", "none");
        if (directionString == "none")
            faceDirection[entity] = FD_None;
        else if (directionString == "forward")
            faceDirection[entity] = FD_Forward;
        else if (directionString == "backward")
            faceDirection[entity] = FD_Backward;

        cooldown[entity] = 0;
        hurt[entity] = false;
        stun[entity] = false;
        stunTimer[entity] = 0;
    }
};
