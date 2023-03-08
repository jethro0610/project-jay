#pragma once
#include "../Entity/Entity.h"
#include <algorithm>

#include "ComponentDesc.h"

const uint16_t PICKUP_DURATION = 10;

struct PickupCProperties {
};

struct PickupComponent {
    int entityId[MAX_ENTITIES];
    bool pickup[MAX_ENTITIES];

    PickupComponent() {
        std::fill_n(entityId, MAX_ENTITIES, -1);
        std::fill_n(pickup, MAX_ENTITIES, false);
    };
    PickupComponent(const PickupComponent&) = delete;
    PickupComponent& operator=(const PickupComponent&) = delete;

    PickupCProperties operator[](int index) {
        return PickupCProperties {
        };
    }

    inline static int ID = 7;
    inline static ComponentDesc description {
        "Pickup Component",
        0,
        {}
    };
};
