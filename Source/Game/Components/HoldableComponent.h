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

    inline static int ID = 2;
    void Load(nlohmann::json& json, uint16_t entity) {
    }
};

