#pragma once
#include "../Entity/Entity.h"
#include "Component.h"
#include <glm.hpp>

struct TerrainModCProperties {
    glm::vec3& position;
    float& radius;
};

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

    TerrainModCProperties operator[](int index) {
        return TerrainModCProperties{
            position[index],
            radius[index]
        };
    }

    inline static int ID = 12;
    void Load(nlohmann::json& data, uint8_t index, uint16_t entity) {
        radius[entity] = data["components"][index]["radius"].get<float>();
    }
};
