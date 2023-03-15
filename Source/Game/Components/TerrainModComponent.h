#pragma once
#include "Component.h"
#include <glm.hpp>

struct TerrainModComponent : public Component {
    // TODO: Remove position and use transform instead
    glm::vec3 position[MAX_ENTITIES];
    float radius[MAX_ENTITIES];
    bool updated[MAX_ENTITIES];

    TerrainModComponent() {
        std::fill_n(position, MAX_ENTITIES, glm::vec3(0.0f));
        std::fill_n(radius, MAX_ENTITIES, 0.0f);
        std::fill_n(updated, MAX_ENTITIES, false);
    };
    TerrainModComponent (const TerrainModComponent &) = delete;
    TerrainModComponent & operator=(const TerrainModComponent &) = delete;

    std::string GetName() const { return "terrain_modifier"; }
    void Load(nlohmann::json& data, EntityID entity) {
        radius[entity] = data["radius"].get<float>();
    }
};
