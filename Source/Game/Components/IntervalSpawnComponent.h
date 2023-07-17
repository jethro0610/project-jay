#pragma once
#include <glm/vec3.hpp>
#include "Component.h"
#include "../../Logging/Logger.h"

const int MAX_INTERVAL_SPAWN_OFFSET = 16;

class IntervalSpawnComponent : public Component {
public:
    std::string entityToSpawn[MAX_ENTITIES];
    int interval[MAX_ENTITIES];
    bool launch[MAX_ENTITIES];

    glm::vec3 offsets[MAX_ENTITIES][MAX_INTERVAL_SPAWN_OFFSET];
    int numOffsets[MAX_ENTITIES];

    int entitiesPerOffset[MAX_ENTITIES];
    EntityID offsetEntity[MAX_ENTITIES][MAX_INTERVAL_SPAWN_OFFSET];

    int timer[MAX_ENTITIES]; 

    IntervalSpawnComponent() {
        std::fill_n(timer, MAX_ENTITIES, 0);
    };
    IntervalSpawnComponent(const IntervalSpawnComponent&) = delete;
    IntervalSpawnComponent& operator=(const IntervalSpawnComponent&) = delete;

    static constexpr std::string GetName() { return "interval_spawner"; }
    static constexpr int GetID() { return 3; }

    void Load(nlohmann::json& data, EntityID entity) {
        entityToSpawn[entity] = GetString(data, "entity", "");
        interval[entity] = GetFloat(data, "interval", 0) * 60;
        launch[entity] = GetBoolean(data, "launch", false);

        if (!data.contains("offsets")) {
            numOffsets[entity] = 1;
            offsets[entity][0] = glm::vec3(0.0f, 0.0f, 0.0f);
        }
        else {
            auto jOffsets = data["offsets"];
            numOffsets[entity] = jOffsets.size();
            for (int i = 0; i < numOffsets[entity]; i++)
                offsets[entity][i] = glm::vec3(jOffsets[i]["x"], jOffsets[i]["y"], jOffsets[i]["z"]);
        }

        // -1 entities per offset means infinite number
        // Need someway to unassign entity, check the offsetEntity to decide
        entitiesPerOffset[entity] = GetInt(data, "entities_per_offset", -1);
    }
};
