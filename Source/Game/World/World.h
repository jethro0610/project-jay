#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>
#include <vector>
#include <unordered_set>
#include <FastNoiseLite.h>
#include "../../Types/FixedVector.h"
#include "TerrainModifier.h"
#include "../../Constants/WorldConstants.h"
class ResourceManager;

class World {
public:
    FixedVector<TerrainModifier, MAX_TERRAIN_MODIFIERS> terrainModifiers_;
    World(ResourceManager& resourceManager);

    float GetTerrainHeight(glm::vec2 position) const;
    float GetDistance(glm::vec3 position) const;

    // Higher epsilon = smoother
    glm::vec3 GetNormal(glm::vec3 position, float epsilon = 1.0f) const;
    glm::vec3 GetNearestInDirection(glm::vec3 start, glm::vec3 direction, uint16_t maxSteps = 32);

    void UpdateDirtyChunks();
    void MarkAllDirty();
    void UpdateTerrainModifiersGPU_P();

private:
    ResourceManager& resourceManager_;
    FastNoiseLite* noise_;
    std::unordered_set<glm::ivec3> dirtyChunks_;

    void GenerateNoiseTexture_P();
    void GenerateMeshGPU_P(glm::ivec3 chunk);
};
