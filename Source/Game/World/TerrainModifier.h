#pragma once
#include <glm.hpp>
const uint16_t MAX_TERRAIN_MODIFIERS = 24;

enum TerrainModType {
    Height 
};

struct TerrainModifier {
    TerrainModType type; 
    glm::vec2 position;
    float height;
    float exponent;
    float range;
};
