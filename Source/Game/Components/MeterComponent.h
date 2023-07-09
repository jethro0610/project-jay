#pragma once
#include "Component.h"

class MeterComponent : public Component {
public:
    int meter[MAX_ENTITIES];
    int maxMeter[MAX_ENTITIES];
    bool destroyOnNone[MAX_ENTITIES];

    MeterComponent() {
        std::fill_n(meter, MAX_ENTITIES, 0);
        std::fill_n(maxMeter, MAX_ENTITIES, INT_MAX);
    };
    MeterComponent(const MeterComponent&) = delete;
    MeterComponent& operator=(const MeterComponent&) = delete;

    static constexpr std::string GetName() { return "meter"; }
    static constexpr int GetID() { return 13; }

    void Load(nlohmann::json& data, EntityID entity) {
        meter[entity] = data["initial_meter"].get<int>();
        if (data.contains("max_meter"))
            maxMeter[entity] = data["max_meter"].get<int>();

        destroyOnNone[entity] = GetBoolean(data, "destroy_on_none");
    }
};
