#pragma once
#include "../Entity/Entity.h"
#include <glm.hpp>

struct TerrainModCProperties {
    glm::vec3 position;
};

struct TerrainModComponent {
    glm::vec3 position[MAX_ENTITIES];

    TerrainModComponent () {
        std::fill_n(position, MAX_ENTITIES, glm::vec3(0.0f));
    };
    TerrainModComponent (const TerrainModComponent &) = delete;
    TerrainModComponent & operator=(const TerrainModComponent &) = delete;

    TerrainModCProperties operator[](int index) {
        return TerrainModCProperties{
            position[index],
        };
    }

    static int ID;
};
