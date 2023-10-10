#pragma once
#include "Component.h"
#include "Collider.h"

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
        pushbox[entity].height = GetFloat(data, "height");
        pushbox[entity].send = GetBoolean(data, "send", true);
        pushbox[entity].recieve = GetBoolean(data, "recieve", true);
    }
};
