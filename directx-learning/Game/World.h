#pragma once
#include "../Types/Transform.h"
#include "../Rendering/RenderTypes.h"
#include <vector>

#define WORLD_RESOLUTION 32
#define DISTANCE_FIELD_SIZE (WORLD_RESOLUTION + 1)

const ivec3 cornerTable[8] = {
    {0, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
    {0, 1 ,0},
    {0, 0, 1},
    {1, 0, 1},
    {1, 1, 1},
    {0, 1, 1},
};

const ivec2 edgeTable[12] = {
    {0, 1},
    {3, 2},
    {4, 5},
    {7, 6},

    {0, 3},
    {1, 2},
    {4, 7},
    {5, 6},

    {0, 4},
    {3, 7},
    {1, 5},
    {2, 6},
};

const ivec3 triangleEdgeTable[3] = {
    {1, 0, 0},  // X
    {0, 1, 0},  // Y
    {0, 0, 1},  // Z
};

const ivec3 triangulationTable[3][4] = {
    {{ 0,  0,  0}, { 0,  0, -1}, { 0, -1, -1}, { 0, -1,  0}},   // X; Why is this LHS and backwards tf.
    {{ 0,  0,  0}, {-1,  0,  0}, {-1,  0, -1}, { 0,  0, -1}},   // Y
    {{ 0,  0,  0}, { 0, -1,  0}, {-1, -1,  0}, {-1,  0,  0}},   // Z
};

class World {
public:
    float distanceField_[DISTANCE_FIELD_SIZE][DISTANCE_FIELD_SIZE][DISTANCE_FIELD_SIZE];
    int indicesDataBuffer_[DISTANCE_FIELD_SIZE][DISTANCE_FIELD_SIZE][DISTANCE_FIELD_SIZE];

    void Temp_FillDistanceField();
    void Temp_Generate(std::vector<WorldVertex>& outVertices, std::vector<uint16_t>& outIndices);
    void Temp_GenerateVertices(std::vector<WorldVertex>& outVertices);
    vec3 Temp_GetNormal(int x, int y, int z);
    void Temp_GenerateIndices(std::vector<uint16_t>& outInidices);
};