#pragma once
#include "../Entity/Entity.h"
#include "Component.h"
#include <algorithm>

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

    void operator[](int index) {
        return; 
    }

    std::string GetName() const { return "pickup"; }
    void Load(nlohmann::json& data, uint16_t entity) {
    }
};
