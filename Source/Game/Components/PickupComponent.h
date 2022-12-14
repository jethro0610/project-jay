#pragma once
#include "../Entity/Entity.h"
#include "../../Types/Transform.h"
#include <algorithm>

const uint16_t PICKUP_DURATION = 10;

struct PickupCProperties {
    float& range;
};

struct PickupComponent {
    int entityId[MAX_ENTITIES];
    float range[MAX_ENTITIES];
    bool pickup[MAX_ENTITIES];

    PickupComponent() {
        std::fill_n(entityId, MAX_ENTITIES, -1);
        std::fill_n(range, MAX_ENTITIES, 0.0f);
        std::fill_n(pickup, MAX_ENTITIES, false);
    };
    PickupComponent(const PickupComponent&) = delete;
    PickupComponent& operator=(const PickupComponent&) = delete;

    PickupCProperties operator[](int index) {
        return PickupCProperties {
            range[index]
        };
    }

    static int ID;
};
