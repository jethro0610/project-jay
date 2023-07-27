#pragma once
#include "Component.h"

class IntervalSpawnComponent : public Component {
public:
    int spawnInterval[MAX_ENTITIES];
    int spawnTimer[MAX_ENTITIES]; 
    std::string entityToSpawn[MAX_ENTITIES];

    IntervalSpawnComponent() {
        std::fill_n(spawnTimer, MAX_ENTITIES, 0);
    };
    IntervalSpawnComponent(const IntervalSpawnComponent&) = delete;
    IntervalSpawnComponent& operator=(const IntervalSpawnComponent&) = delete;

    static constexpr std::string GetName() { return "interval_spawner"; }
    static constexpr int GetID() { return 3; }

    void Load(nlohmann::json& data, EntityID entity) {
        spawnInterval[entity] = GetFloat(data, "interval", 1.0f) * 60;
        entityToSpawn[entity] = GetString(data, "entity", "");
    }
};
