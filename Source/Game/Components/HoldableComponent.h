#pragma once
#include "../Entity/Entity.h"
#include "Component.h"
#include "../../Types/Transform.h"
#include <algorithm>

struct HoldableComponent : public Component {
    HoldableComponent() {};
    HoldableComponent (const HoldableComponent&) = delete;
    HoldableComponent& operator=(const HoldableComponent&) = delete;

    void operator[](int index) {
        return; 
    }

    std::string GetName() const { return "holdable"; }
    void Load(nlohmann::json& data, uint16_t entity) {
    }
};

