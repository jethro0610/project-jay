#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>
#include <vector>
#include <unordered_set>
#include "../../Types/FixedVector.h"
#include "TerrainModifier.h"
#include "../../Constants/WorldConstants.h"

class FastNoiseLite;

class World {
public:
    FixedVector<TerrainModifier, MAX_TERRAIN_MODIFIERS> terrainModifiers_;
    World(FastNoiseLite& noise);

    float GetHeight(glm::vec2 position) const;
    float GetHeight(glm::vec3 position) const;

    glm::vec3 GetNormal(glm::vec2 position, float epsilon = 1.0f) const;
    glm::vec3 GetNormal(glm::vec3 position, float epsilon = 1.0f) const;

    void UpdateDirtyChunks();
    void MarkAllDirty();
    void UpdateTerrainModifiersGPU_P();

private:
    FastNoiseLite& noise_;
    std::unordered_set<glm::ivec3> dirtyChunks_;

    void GenerateNoiseTexture_P();
    void GenerateMeshGPU_P(glm::ivec3 chunk);
};
