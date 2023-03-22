#pragma once
#include "Component.h"
#include <algorithm>

enum SpreadActivatorMode {
    NoSpread,
    Spread,
    Cut
};

struct SpreadActivatorComponent : public Component {
    bool groundOnly[MAX_ENTITIES];
    SpreadActivatorMode mode[MAX_ENTITIES];

    SpreadActivatorComponent() {
        std::fill_n(groundOnly, MAX_ENTITIES, true);
        std::fill_n(mode, MAX_ENTITIES, NoSpread);
    };
    SpreadActivatorComponent(const SpreadActivatorComponent&) = delete;
    SpreadActivatorComponent& operator=(const SpreadActivatorComponent&) = delete;

    std::string GetName() const { return "spread_activator"; }
    void Load(nlohmann::json& data, EntityID entity) {
        groundOnly[entity] = data["ground_only"].get<bool>();
    }
};
