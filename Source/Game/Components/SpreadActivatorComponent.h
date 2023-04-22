#pragma once
#include "Component.h"

class SpreadActivatorComponent : public Component {
public:
    bool groundOnly[MAX_ENTITIES];
    int16_t radius[MAX_ENTITIES]; 
    uint32_t meter[MAX_ENTITIES];

    SpreadActivatorComponent() {
        std::fill_n(groundOnly, MAX_ENTITIES, true);
        std::fill_n(radius, MAX_ENTITIES, 0);
        std::fill_n(meter, MAX_ENTITIES, UINT32_MAX);
    };
    SpreadActivatorComponent(const SpreadActivatorComponent&) = delete;
    SpreadActivatorComponent& operator=(const SpreadActivatorComponent&) = delete;

    static constexpr std::string GetName() { return "spread_activator"; }
    static constexpr uint8_t GetID() { return 7; }

    void Load(nlohmann::json& data, EntityID entity) {
        groundOnly[entity] = data["ground_only"].get<bool>();
        if (data.contains("initial_meter"))
            meter[entity] = data["initial_meter"].get<uint32_t>();
    }
};
