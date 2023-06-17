#pragma once
#include <bitset>
#include "Component.h"

enum BubbleProperties {
    Meteor,
    DestroyOnMeteored,
    SpreadOnMeteored,
    ThrowOnMeteored,
    SeedOnMeteored,
    AttractSeed,
    BubblePropertyCount
};

class BubbleComponent : public Component {
public:
    float radius[MAX_ENTITIES];
    std::bitset<BubblePropertyCount> properties[MAX_ENTITIES];

    BubbleComponent() {
        std::fill_n(radius, MAX_ENTITIES, 0.0f);
        std::fill_n(properties, MAX_ENTITIES, 0);
    };
    BubbleComponent(const BubbleComponent&) = delete;
    BubbleComponent& operator=(const BubbleComponent&) = delete;

    static constexpr std::string GetName() { return "bubble"; }
    static constexpr uint8_t GetID() { return 0; }

    void Load(nlohmann::json& data, EntityID entity) {
        radius[entity] = data["radius"].get<float>();

        properties[entity].set(Meteor, GetBoolean(data, "meteor"));
        properties[entity].set(DestroyOnMeteored, GetBoolean(data, "destroy_on_meteored"));
        properties[entity].set(SpreadOnMeteored, GetBoolean(data, "spread_on_meteored"));
        properties[entity].set(ThrowOnMeteored, GetBoolean(data, "throw_on_meteored"));
        properties[entity].set(SeedOnMeteored, GetBoolean(data, "seed_on_meteored"));
        properties[entity].set(AttractSeed, GetBoolean(data, "attract_seed"));
    }
};
