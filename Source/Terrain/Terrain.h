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

    void Update(const BlobProperties& blob);
    // void GenerateTerrainMap(
    //     const BlobProperties& blob
    // );
    void GenerateTerrainMapSectionThreaded(
        const glm::ivec2& start,
        const glm::ivec2& end,
        int index
    );
    void UpdateTerrainMapTexture();
    int area_;
    vector_contig<TerrainBubble, TerrainBubble::MAX> bubbles_;
    vector_contig<TerrainCurve, TerrainCurve::MAX> curves_;

    Model* nodeModel_;
    Material bubbleMaterial_;
    Material curveMaterial_;
    Material curveControlMaterial_;

private:
    glm::vec2 terrainMap_[TerrainConsts::RESOLUTION][TerrainConsts::RESOLUTION];
    TerrainAffectMap affectMap_;
    Texture* terrainMapTexture_;
    ResourceManager& resourceManager_;

    #ifdef _DEBUG
    int cores_;
    int sectionSize_;
    glm::vec2 terrainMapBack_[TerrainConsts::RESOLUTION][TerrainConsts::RESOLUTION];
    std::vector<std::thread> threads_;
    bool completeThreads_[32];
    std::mutex threadMutexes_[32];
    vector_contig<TerrainBubble, TerrainBubble::MAX> bubblesBack_;
    vector_contig<TerrainCurve, TerrainCurve::MAX> curvesBack_;
    #endif
};
