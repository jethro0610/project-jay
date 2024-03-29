#pragma once
#include <glm/vec2.hpp>
const int MAX_TERRAIN_MODIFIERS = 16;

enum TerrainModType {
    TERRAINMOD_HEIGHT 
};

struct TerrainModifier {
    TerrainModType type; 
    glm::vec2 position;
    float height;
    float exponent;
    float range;
};
