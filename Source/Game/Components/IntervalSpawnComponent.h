#pragma once
#include "Component.h"
#include <algorithm>

struct IntervalSpawnComponent : public Component {
    uint16_t spawnInterval[MAX_ENTITIES];
    uint16_t spawnTimer[MAX_ENTITIES]; 
    std::string entityToSpawn[MAX_ENTITIES];

    IntervalSpawnComponent() {
        std::fill_n(spawnInterval, MAX_ENTITIES, 0);
        std::fill_n(spawnTimer, MAX_ENTITIES, 0);
        std::fill_n(entityToSpawn, MAX_ENTITIES, "");
    };
    IntervalSpawnComponent(const IntervalSpawnComponent&) = delete;
    IntervalSpawnComponent& operator=(const IntervalSpawnComponent&) = delete;

    std::string GetName() const { return "interval_spawner"; }
    void Load(nlohmann::json& data, EntityID entity) {
        spawnInterval[entity] = data["interval"].get<int>();
        entityToSpawn[entity] = data["entity"].get<std::string>();
    }
};
