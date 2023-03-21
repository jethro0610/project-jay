#pragma once
#include "Component.h"
#include <json.hpp>
#include <algorithm>
#include <bitset>

enum BubbleProperties {
    Meteor,
    DestroyOnMeteored,
    SpreadOnMeteored,
    ThrowOnMeteored,
    BubblePropertyCount
};

struct BubbleComponent : public Component {
    float radius[MAX_ENTITIES];
    std::bitset<BubblePropertyCount> properties[MAX_ENTITIES];

    BubbleComponent() {
        std::fill_n(radius, MAX_ENTITIES, 0.0f);
        std::fill_n(properties, MAX_ENTITIES, 0);
    };
    BubbleComponent(const BubbleComponent&) = delete;
    BubbleComponent& operator=(const BubbleComponent&) = delete;

    std::string GetName() const { return "bubble"; }
    void Load(nlohmann::json& data, EntityID entity) {
        radius[entity] = data["radius"].get<float>();

        properties[entity].set(Meteor, GetBoolean(data, "meteor", true));
        properties[entity].set(DestroyOnMeteored, GetBoolean(data, "destroy_on_meteored"));
        properties[entity].set(SpreadOnMeteored, GetBoolean(data, "spread_on_meteored"));
        properties[entity].set(ThrowOnMeteored, GetBoolean(data, "throw_on_meteored"));
    }
};
