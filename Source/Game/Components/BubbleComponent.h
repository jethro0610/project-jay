#pragma once
#include "Component.h"
#include "../Entity/Entity.h"
#include <json.hpp>
#include <algorithm>

struct BubbleCProperties {
    float& radius;
};

struct BubbleComponent : public Component {
    float radius[MAX_ENTITIES];

    BubbleComponent() {
        std::fill_n(radius, MAX_ENTITIES, 0.0f);
    };
    BubbleComponent(const BubbleComponent&) = delete;
    BubbleComponent& operator=(const BubbleComponent&) = delete;

    BubbleCProperties operator[](int index) {
        return BubbleCProperties {
            radius[index]
        };
    }

    std::string GetName() const { return "bubble"; }
    void Load(nlohmann::json& data, uint16_t entity) {
        radius[entity] = data["radius"].get<float>();
    }
};
