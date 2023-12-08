#pragma once
#include "Component.h"
#include "../Collision/Collider.h"

struct Pushbox : public Collider {
    bool send;
    bool recieve;
};

class PushboxComponent: public Component {
public:
    std::array<Pushbox, MAX_ENTITIES> pushbox;
    std::array<bool, MAX_ENTITIES> sentPush;
    std::array<bool, MAX_ENTITIES> recievedPush;

    PushboxComponent() {
        sentPush.fill(false);
        recievedPush.fill(false);
    };
    PushboxComponent(const PushboxComponent&) = delete;
    PushboxComponent& operator=(const PushboxComponent&) = delete;

    static constexpr std::string GetName() { return "pushbox"; }
    static constexpr int GetID() { return 0; }

    void Load(nlohmann::json& data, EntityID entity) {
        pushbox[entity].radius = GetFloat(data, "radius");
        pushbox[entity].top = GetFloat(data, "top", 0.0f);
        pushbox[entity].bottom = GetFloat(data, "bottom", 0.0f);
        pushbox[entity].send = GetBoolean(data, "send", true);
        pushbox[entity].recieve = GetBoolean(data, "recieve", true);
    }
};
