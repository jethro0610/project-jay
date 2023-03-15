#pragma once
#include "Component.h"
#include "../../Types/Transform.h"
#include <algorithm>

struct HoldableComponent : public Component {
    HoldableComponent() {};
    HoldableComponent (const HoldableComponent&) = delete;
    HoldableComponent& operator=(const HoldableComponent&) = delete;

    std::string GetName() const { return "holdable"; }
    void Load(nlohmann::json& data, EntityID entity) {
    }
};

