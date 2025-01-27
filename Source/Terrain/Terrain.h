#pragma once
#include "Shared_TerrainConstants.h"
#include "Rendering/Texture.h"
#include "Rendering/Model.h"
#include "Rendering/Material.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>
#include <vector_contig.h>
#include <nlohmann/json.hpp>

#ifdef _DEBUG
#include "TerrainBubble.h"
#include "TerrainNoise.h"
#endif

class EntityList;
class LevelProperties;
class ResourceManager;

enum TerrainAccuracy {
    TA_Low,
    TA_Normal
};

class Terrain {
public:
    static constexpr int NUM_LODS = 4;
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
    float GetRawHeight(const glm::vec2& position, TerrainAccuracy accuracy = TA_Normal) const;

    glm::vec3 GetNormal(const glm::vec2& position, TerrainAccuracy accuracy = TA_Normal) const;
    glm::vec3 GetNormal(const glm::vec3& position, TerrainAccuracy accuracy = TA_Normal) const;
    glm::vec2 GetDirectionToEdge(const glm::vec2& position, TerrainAccuracy accuracy = TA_Normal) const;
    glm::vec3 GetDirectionToEdge(const glm::vec3& position, TerrainAccuracy accuracy = TA_Normal) const;
    int area_;

    void GenerateTerrainHeights(bool lowRes = false, EntityList* entities = nullptr);
    void GenerateTerrainDistances(EntityList* entities = nullptr);
    void ReloadTerrainDistances(EntityList* entities = nullptr);
    glm::vec3 RaycastTerrain(const glm::vec3& origin, const glm::vec3& direction);
    bool PointIsInSameIsland(const glm::vec3& origin, const glm::vec3& point, float edgeDistance = 0.0f);
    glm::vec3 GetRandomPointInSameIslandFast(const glm::vec3& origin);
    glm::vec3 GetRandomPointInSameIsland(const glm::vec3& origin, float minDist, float maxDist);

    #ifdef _DEBUG
    StaticTerrainModifier& CreateStaticModifier(int typeId, const glm::vec3& pos = glm::vec3(0.0f));
    bool DestroyPendingModifiers();
    void Reset();

    std::string DBG_landMapName_;
    Model* DBG_modifierNodeModel_;
    Material DBG_modifierNodeMaterial_;
    bool DBG_lowRes_;

    TerrainBubble DBG_bubbles_[64];
    TerrainNoise DBG_noises_[64];
    vector_const<StaticTerrainModifier*, 128> DBG_modifiers_;
    #endif

private:
    ResourceManager& resourceManager_;
    glm::vec2 terrainMap_[TerrainConsts::RESOLUTION][TerrainConsts::RESOLUTION];
    Texture* terrainMapTexture_;

    #ifdef _DEBUG
    uint8_t DBG_additiveMap_[TerrainConsts::RESOLUTION][TerrainConsts::RESOLUTION];
    uint8_t DBG_additiveMapLow_[TerrainConsts::RESOLUTION_LOW][TerrainConsts::RESOLUTION_LOW];
    glm::vec2 DBG_terrainMapLow_[TerrainConsts::RESOLUTION_LOW][TerrainConsts::RESOLUTION_LOW];
    Texture* DBG_terrainMapTextureLow_;
    #endif
};
