#pragma once
#include "../Entity/Entity.h"
#include "../../Types/Transform.h"
#include <algorithm>

struct PickupCProperties {
    float& range;
};

struct PickupComponent {
    uint16_t entityId[MAX_ENTITIES];
    float range[MAX_ENTITIES];
    uint16_t timer[MAX_ENTITIES];

    PickupComponent() {
        std::fill_n(entityId, MAX_ENTITIES, -1);
        std::fill_n(range, MAX_ENTITIES, 0.0f);
        std::fill_n(timer, MAX_ENTITIES, 0);
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
