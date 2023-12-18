#pragma once
#include "Shared_TerrainProperties.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>

class Terrain {
public:
    static constexpr float TERRAIN_MESH_SIZE = 64.0f;
    static constexpr float TERRAIN_MESH_DENSITY = 0.5f;
    static constexpr int RESOLUTION = 4096;
    static constexpr int HALF_RESOLUTION = RESOLUTION / 2;

    Terrain();

    float SampleHeightmap(float x, float y, TerrainAccuracy accuracy = TA_Normal) const;
    float SampleHeightmap(const glm::vec2& position, TerrainAccuracy accuracy = TA_Normal) const;

    glm::vec2 GetDistance(const glm::vec2& position, TerrainAccuracy accuracy = TA_Normal) const;
    glm::vec2 GetDistance(const glm::vec3& position, TerrainAccuracy accuracy = TA_Normal) const;

    float GetHeight(const glm::vec2& position, TerrainAccuracy accuracy = TA_Normal) const;
    float GetHeight(const glm::vec3& position, TerrainAccuracy accuracy = TA_Normal) const;

    glm::vec3 GetNormal(const glm::vec2& position, TerrainAccuracy accuracy = TA_Normal) const;
    glm::vec3 GetNormal(const glm::vec3& position, TerrainAccuracy accuracy = TA_Normal) const;

    float* GetHeightmap() const { return (float*)heightmap_; }

    TerrainProperties properties_;

private:
    float heightmap_[RESOLUTION][RESOLUTION];
    float range_;
    float scale_;
};
