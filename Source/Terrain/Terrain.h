#pragma once
#include "Shared_TerrainConstants.h"
#include "Rendering/Texture.h"
#include "Rendering/Model.h"
#include "Rendering/Material.h"
#include "TerrainCurve.h"
#include "TerrainBubble.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>
#include <vector_contig.h>
#include <thread>
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
    int area_;

private:
    ResourceManager& resourceManager_;
    glm::vec2 terrainMap_[TerrainConsts::RESOLUTION][TerrainConsts::RESOLUTION];
    Texture* terrainMapTexture_;

#ifdef _DEBUG
public:
    TerrainBubble* AddBubble(glm::vec3 position);
    TerrainCurve* AddCurve(glm::vec3 position);
    bool DestroyControls();

    void GenerateTerrainHeights(bool lowRes = false, EntityList* entities = nullptr, nlohmann::json* phases = nullptr);
    void GenerateTerrainDistances(EntityList* entities = nullptr, nlohmann::json* phases = nullptr);
    void GenerateTerrainDistanceSection(
        const glm::vec2& start,
        const glm::vec2& end,
        const uint8_t landMap[TerrainConsts::RESOLUTION][TerrainConsts::RESOLUTION],
        const std::vector<glm::ivec2>& edges
    );
    void ReloadTerrainDistances(EntityList* entities = nullptr, nlohmann::json* phases = nullptr);

    std::string DBG_landMapName_;
    vector_contig<TerrainBubble, TerrainBubble::MAX> DBG_bubbles_;
    vector_contig<TerrainCurve, TerrainCurve::MAX> DBG_curves_;

    Model* DBG_nodeModel_;
    Material DBG_bubbleMaterial_;
    Material DBG_curveMaterial_;
    Material DBG_curveControlMaterial_;
    bool DBG_lowRes_;

private:
    uint32_t DBG_affectMap_[TerrainConsts::RESOLUTION][TerrainConsts::RESOLUTION];
    glm::vec2 DBG_terrainMapLow_[TerrainConsts::RESOLUTION_LOW][TerrainConsts::RESOLUTION_LOW];
    uint32_t DBG_affectMapLow_[TerrainConsts::RESOLUTION_LOW][TerrainConsts::RESOLUTION_LOW];
    Texture* DBG_terrainMapTextureLow_;
#endif
};
