#pragma once
#include <glm.hpp>
#include <vector>
#include <set>
#include <FastNoiseLite.h>
#include "../Entity/Entity.h"
#include "../../Constants/WorldConstants.h"
#include "../../Resource/ResourceManager.h"
#include "../../Types/FixedVector.h"
#include "TerrainModifier.h"

const glm::float3 cornerTable[8] = {
    {0.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 1.0f},
    {1.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
    {0.0f, 1.0f, 1.0f},
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
const glm::vec3 triangleEdgeTable[3] = {
    {1.0f, 0.0f, 0.0f},  // X
    {0.0f, 1.0f, 0.0f},  // Y
    {0.0f, 0.0f, 1.0f},  // Z
};
const glm::ivec3 triangulationTable[3][4] = {
    {{ 0,  0,  0}, { 0,  0, -1}, { 0, -1, -1}, { 0, -1,  0}},   // X; Why is this LHS and backwards tf.
    {{ 0,  0,  0}, {-1,  0,  0}, {-1,  0, -1}, { 0,  0, -1}},   // Y
    {{ 0,  0,  0}, { 0, -1,  0}, {-1, -1,  0}, {-1,  0,  0}},   // Z
};

class World {
public:
    World(Entity* entities, ResourceManager& resourceManager);
    ResourceManager& resourceManager_;
    Entity* entities_;
    FixedVector<TerrainModifier, MAX_TERRAIN_MODIFIERS> terrainModifiers_;
    std::set<glm::ivec3> dirtyChunks_;

    float GetTerrainHeight(glm::vec2 position) const;
    float GetDistance(glm::vec3 position) const;
    // Higher epsilon = smoother
    glm::vec3 GetNormal(glm::vec3 position, float epsilon = 1.0f) const;
    glm::vec3 GetNearestInDirection(glm::vec3 start, glm::vec3 direction, uint16_t maxSteps = 32);

    void AddTerrainModifier(TerrainModifier modifier);
    void GetMesh(glm::ivec3 chunk, std::vector<WorldVertex>& outVertices, std::vector<uint16_t>& outIndices);
    void GenerateMeshGPU_P(glm::ivec3 chunk);
    void UpdateModifiersGPU_P();

private:
    FastNoiseLite* noise_;
    void GenerateNoiseTexture_P();
};
