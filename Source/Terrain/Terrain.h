#pragma once
#include "Shared_TerrainProperties.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>

struct NoiseLayer {
    bool active_;
    int seed_;
    float frequency_;
    float multiplier_;
    float exponent_;

    NoiseLayer() {
        active_ = false;
        seed_ = 1337;
        frequency_ = 0.25f;
        multiplier_ = 1.0f;
        exponent_ = 1.0f;
    }
};

struct BlobProperties {
    int seed_;
    float frequency_;
    float minRadius_;
    float maxRadius_;
};

class Terrain {
public:
    static constexpr float TERRAIN_MESH_SIZE = 64.0f;
    static constexpr float TERRAIN_MESH_DENSITY = 0.5f;
    static constexpr int RESOLUTION = 4096;
    static constexpr int HALF_RESOLUTION = RESOLUTION / 2;
    static constexpr float WORLD_TO_TERRAIN_SCALAR = RESOLUTION / TERRAIN_RANGE;

    Terrain();

    glm::vec2 SampleTerrainMap(float x, float y, TerrainAccuracy accuracy = TA_Normal) const;
    glm::vec2 SampleTerrainMap(const glm::vec2& position, TerrainAccuracy accuracy = TA_Normal) const;

    glm::vec2 GetDistance(const glm::vec2& position, TerrainAccuracy accuracy = TA_Normal) const;
    glm::vec2 GetDistance(const glm::vec3& position, TerrainAccuracy accuracy = TA_Normal) const;

    float GetHeight(const glm::vec2& position, TerrainAccuracy accuracy = TA_Normal) const;
    float GetHeight(const glm::vec3& position, TerrainAccuracy accuracy = TA_Normal) const;

    glm::vec3 GetNormal(const glm::vec2& position, TerrainAccuracy accuracy = TA_Normal) const;
    glm::vec3 GetNormal(const glm::vec3& position, TerrainAccuracy accuracy = TA_Normal) const;

    float* GetTerrainMap() const { return (float*)terrainMap_; }

    void GenerateTerrainMap();

    TerrainProperties properties_;

    std::array<NoiseLayer, 4> noiseLayers_;
    BlobProperties blobProperties_;

private:
    glm::vec2 terrainMap_[RESOLUTION][RESOLUTION];
};
