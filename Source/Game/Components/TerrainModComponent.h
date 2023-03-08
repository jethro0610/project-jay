#pragma once
#include "../Entity/Entity.h"
#include <glm.hpp>

#include "ComponentDesc.h"

struct TerrainModCProperties {
    glm::vec3& position;
    float& radius;
};

struct TerrainModComponent {
    // TODO: Remove position and use transform instead
    glm::vec3 position[MAX_ENTITIES];
    float radius[MAX_ENTITIES];
    bool updated[MAX_ENTITIES];

    TerrainModComponent () {
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
    inline static ComponentDesc description {
        "Terrain Modifier",
        1,
        {
            {"Radius", "float"}
        }
    };
};
