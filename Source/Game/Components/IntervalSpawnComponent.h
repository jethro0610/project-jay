#pragma once
#include "Component.h"

class IntervalSpawnComponent : public Component {
public:
    int spawnInterval[MAX_ENTITIES];
    int spawnTimer[MAX_ENTITIES]; 
    std::string entityToSpawn[MAX_ENTITIES];

    IntervalSpawnComponent() {
        std::fill_n(spawnInterval, MAX_ENTITIES, 0);
        std::fill_n(spawnTimer, MAX_ENTITIES, 0);
        std::fill_n(entityToSpawn, MAX_ENTITIES, "");
    };
    IntervalSpawnComponent(const IntervalSpawnComponent&) = delete;
    IntervalSpawnComponent& operator=(const IntervalSpawnComponent&) = delete;

    static constexpr std::string GetName() { return "interval_spawner"; }
    static constexpr int GetID() { return 3; }

    void Load(nlohmann::json& data, EntityID entity) {
        spawnInterval[entity] = data["interval"].get<int>();
        entityToSpawn[entity] = data["entity"].get<std::string>();
    }
};
