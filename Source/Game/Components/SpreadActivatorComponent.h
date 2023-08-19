#pragma once
#include "Component.h"

class SpreadActivatorComponent : public Component {
public:
    std::array<bool, MAX_ENTITIES> groundOnly;
    std::array<int, MAX_ENTITIES> radius; 
    std::array<int, MAX_ENTITIES> amount;

    SpreadActivatorComponent() {
        radius.fill(0);
        amount.fill(INT_MAX);
    };
    SpreadActivatorComponent(const SpreadActivatorComponent&) = delete;
    SpreadActivatorComponent& operator=(const SpreadActivatorComponent&) = delete;

    static constexpr std::string GetName() { return "spread_activator"; }
    static constexpr int GetID() { return 7; }

    void Load(nlohmann::json& data, EntityID entity) {
        groundOnly[entity] = GetBoolean(data, "ground_only", true);
    }
};
