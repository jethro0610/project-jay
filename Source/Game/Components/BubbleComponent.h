#pragma once
#include "Component.h"
#include <json.hpp>
#include <algorithm>

struct BubbleComponent : public Component {
    float radius[MAX_ENTITIES];

    BubbleComponent() {
        std::fill_n(radius, MAX_ENTITIES, 0.0f);
    };
    BubbleComponent(const BubbleComponent&) = delete;
    BubbleComponent& operator=(const BubbleComponent&) = delete;

    std::string GetName() const { return "bubble"; }
    void Load(nlohmann::json& data, EntityID entity) {
        radius[entity] = data["radius"].get<float>();
    }
};
