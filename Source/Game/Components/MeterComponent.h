#pragma once
#include "Component.h"

class MeterComponent : public Component {
public:
    std::array<int, MAX_ENTITIES> meter;
    std::array<int, MAX_ENTITIES> maxMeter;
    std::array<bool, MAX_ENTITIES> destroyOnNone;

    MeterComponent() {
        meter.fill(0);
        maxMeter.fill(INT_MAX);
    };
    MeterComponent(const MeterComponent&) = delete;
    MeterComponent& operator=(const MeterComponent&) = delete;

    static constexpr std::string GetName() { return "meter"; }
    static constexpr int GetID() { return 13; }

    void Load(nlohmann::json* data, EntityID entity) {
        meter[entity] = GetInt(data, "initial_meter");
        maxMeter[entity] = GetInt(data, "max_meter");
        destroyOnNone[entity] = GetBoolean(data, "destroy_on_none");
    }
};
