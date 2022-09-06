#pragma once
#include <glm.hpp>
#include "../Rendering/RenderTypes.h"
#include <vector>
#include "WorldConstants.h"
#include <FastNoiseLite.h>

const glm::ivec3 cornerTable[8] = {
    {0, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
    {0, 1 ,0},
    {0, 0, 1},
    {1, 0, 1},
    {1, 1, 1},
    {0, 1, 1},
};
const glm::ivec2 edgeTable[12] = {
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
const glm::ivec3 triangleEdgeTable[3] = {
    {1, 0, 0},  // X
    {0, 1, 0},  // Y
    {0, 0, 1},  // Z
};
const glm::ivec3 triangulationTable[3][4] = {
    {{ 0,  0,  0}, { 0,  0, -1}, { 0, -1, -1}, { 0, -1,  0}},   // X; Why is this LHS and backwards tf.
    {{ 0,  0,  0}, {-1,  0,  0}, {-1,  0, -1}, { 0,  0, -1}},   // Y
    {{ 0,  0,  0}, { 0, -1,  0}, {-1, -1,  0}, {-1,  0,  0}},   // Z
};

class World {
public:
    World();

    float GetDistance(glm::vec3 position) const;
    // Higher epsilon = smoother
    glm::vec3 GetNormal(glm::vec3 position, float epsilon = 1.0f) const;

    // NOTE: These functions fill the local distance cache based on the coordinate
    void GetMesh(glm::ivec3 coordinates, std::vector<WorldVertex>& outVertices, std::vector<uint16_t>& outIndices);
    void GetMeshGPUCompute(void* graphicsResources, glm::ivec3 coordinates, std::vector<WorldVertex>& outVertices, std::vector<uint16_t>& outIndices);

    static float Lerp(float a, float b, float t);
private:
    FastNoiseLite* noise_;

    // This data channel is necessary because it would otherwise be too big to work in the stack
    int indicesDataChannel_[WORLD_RESOLUTION][WORLD_RESOLUTION][WORLD_RESOLUTION];
    float localDistanceCache_[DISTANCE_CACHE_SIZE][DISTANCE_CACHE_SIZE][DISTANCE_CACHE_SIZE];

    void FillLocalDistanceCache(glm::ivec3 coordinates);

    // NOTE: These functions write to the indices data channell
    void GetMeshVerticesCPU(glm::ivec3 coordinates, std::vector<WorldVertex>& outVertices);
    void GetMeshVerticesGPU_P(void* graphicsResources, glm::ivec3 coordinates, std::vector<WorldVertex>& outVertices);

    void GetMeshIndices(glm::ivec3 coordinates, std::vector<uint16_t>& outInidices);
};
