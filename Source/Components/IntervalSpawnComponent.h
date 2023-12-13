#pragma once
#include "Component.h"
#include "Resource/ResourceManager.h"
#include <glm/vec3.hpp>

class IntervalSpawnComponent : public Component {
public:
    ResourceManager* resourceManager;

    std::array<EntityDescription*, MAX_ENTITIES> entityToSpawn;
    std::array<int, MAX_ENTITIES> interval;
    std::array<float, MAX_ENTITIES> radius;
    std::array<bool, MAX_ENTITIES> planar;

    std::array<std::vector<glm::vec3>, MAX_ENTITIES> offsets;

    std::array<int, MAX_ENTITIES> entitiesPerOffset;
    std::array<std::vector<EntityID>, MAX_ENTITIES> offsetEntity;

    std::array<int, MAX_ENTITIES> timer; 

    IntervalSpawnComponent() {
        timer.fill(0);
    };
    IntervalSpawnComponent(const IntervalSpawnComponent&) = delete;
    IntervalSpawnComponent& operator=(const IntervalSpawnComponent&) = delete;

    static constexpr std::string GetName() { return "interval_spawner"; }
    static constexpr int GetID() { return 3; }

    void Load(nlohmann::json& data, EntityID entity) {
        offsets[entity].clear();
        offsets[entity].shrink_to_fit();
        
        entityToSpawn[entity] = resourceManager->GetEntityDescription(GetString(data, "entity", ""));
        interval[entity] = GetFloat(data, "interval", 1.0f) * 60;
        radius[entity] = GetFloat(data, "radius", 0.0f);
        planar[entity] = GetBoolean(data, "planar");

        if (!data.contains("offsets")) {
            offsets[entity].push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        }
        else {
            auto& jOffsets = data["offsets"];
            for (auto& jOffset : jOffsets)
                offsets[entity].push_back(glm::vec3(jOffset["x"], jOffset["y"], jOffset["z"]));
        }

        // -1 entities per offset means infinite number
        // Need someway to unassign entity, check the offsetEntity to decide
        entitiesPerOffset[entity] = GetInt(data, "entities_per_offset", -1);
    }
};
