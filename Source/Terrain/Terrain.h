#pragma once
#include "Shared_TerrainConstants.h"
#include "BlobProperties.h"
#include "NoiseLayer.h"
#include "Rendering/Texture.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>
#include <FastNoiseLite.h>

class EntityList;
class LevelProperties;
class ResourceManager;

enum TerrainAccuracy {
    TA_Low,
    TA_Normal
};

class Terrain {
public:
    USE_TERRAIN_CONSTANTS
    Terrain(
        LevelProperties& levelProperties,
        ResourceManager& resourceManager
    );

    glm::vec2 SampleTerrainMap(float x, float y, TerrainAccuracy accuracy = TA_Normal) const;
    glm::vec2 SampleTerrainMap(const glm::vec2& position, TerrainAccuracy accuracy = TA_Normal) const;

    glm::vec2 GetDistance(const glm::vec2& position, TerrainAccuracy accuracy = TA_Normal) const;
    glm::vec2 GetDistance(const glm::vec3& position, TerrainAccuracy accuracy = TA_Normal) const;

    float GetHeight(const glm::vec2& position, TerrainAccuracy accuracy = TA_Normal) const;
    float GetHeight(const glm::vec3& position, TerrainAccuracy accuracy = TA_Normal) const;

    glm::vec3 GetNormal(const glm::vec2& position, TerrainAccuracy accuracy = TA_Normal) const;
    glm::vec3 GetNormal(const glm::vec3& position, TerrainAccuracy accuracy = TA_Normal) const;

    void GenerateTerrainMap(
        const std::array<NoiseLayer, NoiseLayer::MAX>& noiseLayers,
        const BlobProperties& blob
    );
    void GenerateTerrainMap(
        const std::array<NoiseLayer, NoiseLayer::MAX>& noiseLayers,
        const BlobProperties& blob,
        EntityList& entities
    );
    void GenerateTerrainMapSection(
        const std::array<NoiseLayer, NoiseLayer::MAX>& noiseLayers,
        const BlobProperties& blob,
        const FastNoiseLite& blobNoise,
        const std::array<FastNoiseLite, 4>& noises,
        const glm::ivec2& start,
        const glm::ivec2& end
    );
    void UpdateTerrainMapTexture();
    int area_;

private:
    glm::vec2 terrainMap_[RESOLUTION][RESOLUTION];
    Texture* terrainMapTexture_;
    ResourceManager& resourceManager_;
};
