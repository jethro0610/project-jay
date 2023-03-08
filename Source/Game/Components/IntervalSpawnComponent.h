#pragma once
#include "../Entity/Entity.h"
#include <algorithm>

#include "ComponentDesc.h"

struct IntervalSpawnCProperties {
    uint16_t& spawnInterval;
};

struct IntervalSpawnComponent {
    uint16_t spawnInterval[MAX_ENTITIES];
    uint16_t spawnTimer[MAX_ENTITIES]; 

    IntervalSpawnComponent() {
        std::fill_n(spawnInterval, MAX_ENTITIES, 0);
        std::fill_n(spawnTimer, MAX_ENTITIES, 0);
    };
    IntervalSpawnComponent(const IntervalSpawnComponent&) = delete;
    IntervalSpawnComponent& operator=(const IntervalSpawnComponent&) = delete;

    IntervalSpawnCProperties operator[](int index) {
        return IntervalSpawnCProperties {
            spawnInterval[index]
        };
    }

    inline static int ID = 3;
    inline static ComponentDesc description {
        "Interval Spawner Component",
        1,
        {
            {"Spawn Interval", "uint16_t"}
        }
    };
};
