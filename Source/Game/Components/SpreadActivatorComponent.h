#pragma once
#include "Component.h"
#include <algorithm>

struct SpreadActivatorComponent : public Component {
    bool groundOnly[MAX_ENTITIES];
    bool active[MAX_ENTITIES];

    SpreadActivatorComponent() {
        std::fill_n(groundOnly, MAX_ENTITIES, true);
        std::fill_n(active, MAX_ENTITIES, false);
    };
    SpreadActivatorComponent(const SpreadActivatorComponent&) = delete;
    SpreadActivatorComponent& operator=(const SpreadActivatorComponent&) = delete;

    std::string GetName() const { return "spread_activator"; }
    void Load(nlohmann::json& data, EntityID entity) {
        groundOnly[entity] = data["ground_only"].get<bool>();
    }
};
