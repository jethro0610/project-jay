#pragma once
#include "Component.h"

class SpreadActivatorComponent : public Component {
public:
    bool groundOnly[MAX_ENTITIES];
    int radius[MAX_ENTITIES]; 
    int amount[MAX_ENTITIES];

    SpreadActivatorComponent() {
        std::fill_n(groundOnly, MAX_ENTITIES, true);
        std::fill_n(radius, MAX_ENTITIES, 0);
        std::fill_n(amount, MAX_ENTITIES, INT_MAX);
    };
    SpreadActivatorComponent(const SpreadActivatorComponent&) = delete;
    SpreadActivatorComponent& operator=(const SpreadActivatorComponent&) = delete;

    static constexpr std::string GetName() { return "spread_activator"; }
    static constexpr int GetID() { return 7; }

    void Load(nlohmann::json& data, EntityID entity) {
        groundOnly[entity] = data["ground_only"].get<bool>();
    }
};
