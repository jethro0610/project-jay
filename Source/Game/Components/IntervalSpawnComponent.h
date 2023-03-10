#pragma once
#include "../Entity/Entity.h"
#include "Component.h"
#include <algorithm>

struct IntervalSpawnCProperties {
    uint16_t& spawnInterval;
};

struct IntervalSpawnComponent : public Component {
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
    void Load(nlohmann::json& data, uint16_t entity) {
        spawnInterval[entity] = (uint16_t)data["spawnInterval"].get<double>();
    }
};
