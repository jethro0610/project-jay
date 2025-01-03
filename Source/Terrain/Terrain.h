#pragma once
#include "Shared_TerrainConstants.h"
#include "Rendering/Texture.h"
#include "Rendering/Model.h"
#include "Rendering/Material.h"
#include "TerrainCurve.h"
#include "TerrainBubble.h"
#include "TerrainNoise.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>
#include <vector_contig.h>
#include <nlohmann/json.hpp>

class EntityList;
class LevelProperties;
class ResourceManager;

enum TerrainAccuracy {
    TA_Low,
    TA_Normal
};

class Terrain {
public:
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
    glm::vec3 GetDirectionToEdge(const glm::vec2& position, TerrainAccuracy accuracy = TA_Normal) const;
    glm::vec3 GetDirectionToEdge(const glm::vec3& position, TerrainAccuracy accuracy = TA_Normal) const;
    int area_;

    static constexpr int NUM_LODS = 4;

private:
    ResourceManager& resourceManager_;
    glm::vec2 terrainMap_[TerrainConsts::RESOLUTION][TerrainConsts::RESOLUTION];
    Texture* terrainMapTexture_;

#ifdef _DEBUG
public:
    TerrainBubble* AddBubble(glm::vec3 position);
    TerrainCurve* AddCurve(glm::vec3 position);
    TerrainNoise* AddNoise(glm::vec3 position);
    bool DestroyControls();

    void GenerateTerrainHeights(bool lowRes = false, EntityList* entities = nullptr);
    void GenerateTerrainDistances(EntityList* entities = nullptr);
    void ReloadTerrainDistances(EntityList* entities = nullptr);
    glm::vec3 RaycastTerrain(const glm::vec3& origin, const glm::vec3& direction);
    bool PointIsInSameIsland(const glm::vec3& origin, const glm::vec3& point, float edgeDistance = 0.0f);
    glm::vec3 GetRandomPointInSameIslandFast(const glm::vec3& origin);
    glm::vec3 GetRandomPointInSameIsland(const glm::vec3& origin, float minDist, float maxDist);

    std::string DBG_landMapName_;
    vector_contig<TerrainBubble, TerrainBubble::MAX> DBG_bubbles_;
    vector_contig<TerrainCurve, TerrainCurve::MAX> DBG_curves_;
    vector_contig<TerrainNoise, TerrainNoise::MAX> DBG_noises_;

    Model* DBG_nodeModel_;
    Material DBG_bubbleMaterial_;
    Material DBG_curveMaterial_;
    Material DBG_curveControlMaterial_;
    Material DBG_noiseMaterial_;
    bool DBG_lowRes_;

private:
    uint8_t DBG_additiveMap_[TerrainConsts::RESOLUTION][TerrainConsts::RESOLUTION];
    uint8_t DBG_additiveMapLow_[TerrainConsts::RESOLUTION_LOW][TerrainConsts::RESOLUTION_LOW];
    uint32_t DBG_affectMap_[TerrainConsts::RESOLUTION][TerrainConsts::RESOLUTION];
    glm::vec2 DBG_terrainMapLow_[TerrainConsts::RESOLUTION_LOW][TerrainConsts::RESOLUTION_LOW];
    uint32_t DBG_affectMapLow_[TerrainConsts::RESOLUTION_LOW][TerrainConsts::RESOLUTION_LOW];
    Texture* DBG_terrainMapTextureLow_;

#endif
};
