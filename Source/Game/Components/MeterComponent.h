#pragma once
#include "../Entity/Entity.h"
#include "../../Types/Transform.h"
#include <algorithm>

struct MeterCProperties {
};

struct MeterComponent {
    float meter[MAX_ENTITIES];

    MeterComponent() {
        std::fill_n(meter, MAX_ENTITIES, 0.0f);
    };
    MeterComponent(const MeterComponent&) = delete;
    MeterComponent& operator=(const MeterComponent&) = delete;

    MeterCProperties operator[](int index) {
        return MeterCProperties {
        };
    }

    static int ID;
};
