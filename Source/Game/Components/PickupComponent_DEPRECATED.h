#pragma once
#include "Component.h"

const uint16_t PICKUP_DURATION = 10;

struct PickupComponent : public Component {
    int entityId[MAX_ENTITIES];
    bool pickup[MAX_ENTITIES];

    PickupComponent() {
        std::fill_n(entityId, MAX_ENTITIES, -1);
        std::fill_n(pickup, MAX_ENTITIES, false);
    };
    PickupComponent(const PickupComponent&) = delete;
    PickupComponent& operator=(const PickupComponent&) = delete;

    static constexpr std::string GetName() { return "pickup"; }
    static constexpr uint8_t GetID() { return 5; }

    void Load(nlohmann::json& data, EntityID entity) {
    }
};
