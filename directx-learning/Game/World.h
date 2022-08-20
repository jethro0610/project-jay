#pragma once
#include "../Types/Transform.h"
#include "../Rendering/RenderTypes.h"
#include <vector>
#include "WorldConstants.h"

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
    World();
    // This data channel is necessary because it would otherwise be too big to work in the stack
    int indicesDataChannel_[MESH_ITERATION_SIZE][MESH_ITERATION_SIZE][MESH_ITERATION_SIZE];

    bool dirtyCoordinates[MAX_X_COORDINATES][MAX_Y_COORDINATES][MAX_Z_COORDINATES];

    float GetDistance(vec3 position) const;
    vec3 GetNormal(vec3 position, float epsilon = 1.0f) const;
    void GetMesh(ivec3 coordinates, std::vector<WorldVertex>& outVertices, std::vector<uint16_t>& outIndices);

private:
    void GetMeshVertices(ivec3 coordinates, std::vector<WorldVertex>& outVertices);
    void GetMeshIndices(ivec3 coordinates, std::vector<uint16_t>& outInidices);
};