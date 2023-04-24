#pragma once
#include "Component.h"

class MeterComponent : public Component {
public:
    uint16_t meter[MAX_ENTITIES];
    uint16_t maxMeter[MAX_ENTITIES];

    MeterComponent() {
        std::fill_n(meter, MAX_ENTITIES, 0);
        std::fill_n(maxMeter, MAX_ENTITIES, UINT16_MAX);
    };
    MeterComponent(const MeterComponent&) = delete;
    MeterComponent& operator=(const MeterComponent&) = delete;

    static constexpr std::string GetName() { return "meter"; }
    static constexpr uint8_t GetID() { return 13; }

    void Load(nlohmann::json& data, EntityID entity) {
        meter[entity] = data["initial_meter"].get<uint16_t>();
        if (data.contains("max_meter"))
            maxMeter[entity] = data["max_meter"].get<uint16_t>();
    }
};
