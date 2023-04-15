#pragma once
#include <glm.hpp>
#include <vector>
#include <unordered_set>
#include <FastNoiseLite.h>
#include "../Entity/Entity.h"
#include "../../Constants/WorldConstants.h"
#include "../../Resource/ResourceManager.h"
#include "../../Types/FixedVector.h"
#include "TerrainModifier.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/hash.hpp>

class World {
public:
    World(ResourceManager& resourceManager);

    float GetTerrainHeight(glm::vec2 position) const;
    float GetDistance(glm::vec3 position) const;
    // Higher epsilon = smoother
    glm::vec3 GetNormal(glm::vec3 position, float epsilon = 1.0f) const;
    glm::vec3 GetNearestInDirection(glm::vec3 start, glm::vec3 direction, uint16_t maxSteps = 32);

    void AddTerrainModifier(TerrainModifier modifier);
    void GetMesh(glm::ivec3 chunk, std::vector<WorldVertex>& outVertices, std::vector<uint16_t>& outIndices);

    void UpdateDirtyChunks();

private:
    ResourceManager& resourceManager_;

    FastNoiseLite* noise_;
    FixedVector<TerrainModifier, MAX_TERRAIN_MODIFIERS> terrainModifiers_;
    std::unordered_set<ivec3> dirtyChunks_[2];

    void GenerateNoiseTexture_P();
    void GenerateMeshGPU_P(glm::ivec3 chunk);
    void UpdateModifiersGPU_P();
};
