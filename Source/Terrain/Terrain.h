#pragma once
#include "Shared_TerrainConstants.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>

struct NoiseLayer {
    bool active;
    int seed;
    float frequency;
    float multiplier;
    float exponent;

    NoiseLayer() {
        active = false;
        seed = 1337;
        frequency = 0.15f;
        multiplier = 12.0f;
        exponent = 1.0f;
    }
};

struct BlobProperties {
    int seed;
    float frequency;
    float minRadius;
    float maxRadius;

    BlobProperties() {
        seed = 1337;
        frequency = 100.0f;
        minRadius = 150.0f;
        maxRadius = 150.0f;
    }
};

enum TerrainAccuracy {
    TA_Low,
    TA_Normal
};

class Terrain {
public:
    USE_TERRAIN_CONSTANTS
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

    std::array<NoiseLayer, 4> noiseLayers_;
    BlobProperties blobProperties_;

private:
    glm::vec2 terrainMap_[RESOLUTION][RESOLUTION];
};
