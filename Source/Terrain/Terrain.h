#pragma once
#include "Shared_TerrainConstants.h"
#include "BlobProperties.h"
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

    bool DestroyControls();

    void GenerateTerrainHeights(bool lowRes = false, EntityList* entities = nullptr);
    void GenerateTerrainDistances();

    TerrainBubble* AddBubble(glm::vec3 position);
    TerrainCurve* AddCurve(glm::vec3 position);

    int area_;
    vector_contig<TerrainBubble, TerrainBubble::MAX> bubbles_;
    vector_contig<TerrainCurve, TerrainCurve::MAX> curves_;

    Model* nodeModel_;
    Material bubbleMaterial_;
    Material curveMaterial_;
    Material curveControlMaterial_;
    bool lowRes_;

private:
    glm::vec2 terrainMap_[TerrainConsts::RESOLUTION][TerrainConsts::RESOLUTION];
    uint32_t affectMap_[TerrainConsts::RESOLUTION][TerrainConsts::RESOLUTION];
    Texture* terrainMapTexture_;
    ResourceManager& resourceManager_;

    enum RenderBlobBits {
        BB_GeneratedAdd,
        BB_GeneratedSubtract,
        BB_DrawnAdd,
        BB_DrawnSubtract,
    };
    bool blobDraws_[TerrainConsts::RESOLUTION][TerrainConsts::RESOLUTION];
    bool blobSubs_[TerrainConsts::RESOLUTION][TerrainConsts::RESOLUTION];
    bool blobMap_[TerrainConsts::RESOLUTION][TerrainConsts::RESOLUTION];
    uint8_t renderBlobMap_[TerrainConsts::RESOLUTION][TerrainConsts::RESOLUTION];

    #ifdef _DEBUG
    glm::vec2 terrainMapLow_[TerrainConsts::RESOLUTION_LOW][TerrainConsts::RESOLUTION_LOW];
    uint32_t affectMapLow_[TerrainConsts::RESOLUTION_LOW][TerrainConsts::RESOLUTION_LOW];
    Texture* terrainMapTextureLow_;
    #endif
};
