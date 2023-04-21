#pragma once
#include "Component.h"

struct HoldableComponent : public Component {
    HoldableComponent() {};
    HoldableComponent (const HoldableComponent&) = delete;
    HoldableComponent& operator=(const HoldableComponent&) = delete;

    static constexpr std::string GetName() { return "holdable"; }
    static constexpr uint8_t GetID() { return 2; }

    void Load(nlohmann::json& data, EntityID entity) {
    }
};

