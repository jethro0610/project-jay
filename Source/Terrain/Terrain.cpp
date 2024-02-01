#include "Terrain.h"
#include "Shared_TerrainFuncs.h"
#include "Logging/Logger.h"
#include "Level/LevelProperties.h"
#include "Resource/ResourceManager.h"
#include "Entity/EntityList.h"
#include "Helpers/Ease.h"
#include <fstream>
#include <FastNoiseLite.h>
#include <glm/gtx/compatibility.hpp>
#include <thread>
#include <vector_contig.h>
using namespace glm;
using namespace TerrainConsts;

Terrain::Terrain(
    LevelProperties& levelProperties,
    ResourceManager& resourceManager
):
resourceManager_(resourceManager)
{
    lowRes_ = true;

    bubbles_.clear();
    curves_.clear();
    for (int x = 0; x < RESOLUTION; x++) {
    for (int y = 0; y < RESOLUTION; y++) {
        terrainMap_[x][y] = vec2(0.0f);
        terrainMapLow_[x][y] = vec2(0.0f);
    } }

    nodeModel_ = resourceManager.GetModel("st_default");
    bubbleMaterial_.shader = resourceManager.GetShader("vs_static", "fs_color");
    bubbleMaterial_.properties.color = vec4(0.0f, 1.0f, 0.0f, 0.5f);
    curveMaterial_.shader = resourceManager.GetShader("vs_static", "fs_color");
    curveMaterial_.properties.color = vec4(0.0f, 0.0f, 1.0f, 0.5f);
    curveControlMaterial_.shader = resourceManager.GetShader("vs_static", "fs_color_front");
    curveControlMaterial_.properties.color = vec4(0.5f, 0.0f, 1.0f, 0.5f);
}

void Terrain::GenerateTerrainDistanceSection(
    const glm::vec2& start,
    const glm::vec2& end,
    const uint8_t blobMap[RESOLUTION][RESOLUTION],
    const std::vector<glm::ivec2>& edges
) {
    for (int x = start.x; x < end.x; x++) {
    for (int y = start.y; y < end.y; y++) {
        float distance = INFINITY;
        float multiplier = blobMap[x][y] ? -1.0f : 1.0f;
        for (const glm::ivec2& edge : edges) {
            float dx = edge.x - x;
            float dy = edge.y - y;
            distance = std::min(dx * dx + dy * dy, distance);
        }
        terrainMap_[y][x].x = sqrt(distance) * multiplier;
    }}
}

void Terrain::GenerateTerrainDistances(const std::string& blob) {
    uint8_t blobMap[RESOLUTION][RESOLUTION];
    std::ifstream blobTexture("./blobs/" + blob + ".blb");
    DEBUGLOG(blob);
    blobTexture.read((char*)blobMap, RESOLUTION * RESOLUTION * sizeof(uint8_t));
    blobTexture.close();

    std::vector<glm::ivec2> edges;
    edges.reserve(RESOLUTION * RESOLUTION);
    for (int x = 0; x < RESOLUTION; x++) {
    for (int y = 0; y < RESOLUTION; y++) {
        if (!blobMap[x][y])
            continue;

        if (!blobMap[max(x - 1, 0)][y])
            edges.push_back({x, y});
        else if (!blobMap[min(x + 1, RESOLUTION - 1)][y])
            edges.push_back({x, y});
        else if (!blobMap[x][max(y - 1, 0)])
            edges.push_back({x, y});
        else if (!blobMap[x][min(y + 1, RESOLUTION - 1)])
            edges.push_back({x, y});
    }}

    std::vector<std::thread> threads;
    int cores = std::thread::hardware_concurrency();
    int sectionSize = RESOLUTION / cores;

    for (int i = 0; i < cores; i++) {
        int add = (i == cores - 1 && RESOLUTION % cores != 0) ? 1 : 0;
        threads.push_back(std::thread(
                &Terrain::GenerateTerrainDistanceSection,
                this, 
                ivec2(i * sectionSize, 0),
                ivec2((i + 1) * sectionSize + add, RESOLUTION),
                blobMap,
                edges
            )
        );
    }
    for (std::thread& thread : threads)
        thread.join();

    resourceManager_.UpdateTerrainMapTexture((glm::vec2*)terrainMap_);
}

TerrainBubble* Terrain::AddBubble(vec3 position) {
    int bubbleIdx = bubbles_.push_back({vec4(position, 50.0f), false, false});
    return &bubbles_[bubbleIdx];
}

TerrainCurve* Terrain::AddCurve(vec3 position) {
    vec4 pos = vec4(position, 50.0f);
    TerrainCurve curve;
    curve.destroy_ = false;
    curve.DBG_selectedPoint_ = -1;
    for (int i = 0; i < 4; i++) {
        curve.points[i] = pos + vec4(i * 50.0f, 0.0f, 0.0f, 0.0f);
    }
    int curveIdx = curves_.push_back(curve);
    return &curves_[curveIdx];
}

bool Terrain::DestroyControls() {
    bool destroyed = false;
    int i = 0;
    while(i < bubbles_.size()) {
        while (i < bubbles_.size() && bubbles_[i].destroy_) {
            destroyed = true;
            bubbles_[i].destroy_ = false;
            bubbles_.remove(i);
        }
        i++;
    }

    while(i < curves_.size()) {
        while (i < curves_.size() && curves_[i].destroy_) {
            destroyed = true;
            curves_[i].destroy_ = false;
            curves_.remove(i);
        }
        i++;
    }

    return destroyed;
}

struct InverseInfluence {
    float inverseWeight;
    float height;
};

template <const int RES>
void BaseGenerateTerrainHeightsSection(
    const glm::vec2& start,
    const glm::vec2& end,
    glm::vec2 terrainMap[RES][RES],
    uint32_t affectMap[RES][RES],
    const vector_contig<TerrainBubble, TerrainBubble::MAX>& bubbles,
    const vector_contig<TerrainCurve, TerrainCurve::MAX>& curves
) {
    const int HALF_RES = RES * 0.5f;
    const float WORLD_TO_TERRAIN = RES / RANGE;

    for (int x = start.x; x < end.x; x++) {
    for (int y = start.y; y < end.y; y++) {
        terrainMap[y][x].y = 0.0f;

        float wX = x - HALF_RES;
        wX /= WORLD_TO_TERRAIN;
        float wY = y - HALF_RES;
        wY /= WORLD_TO_TERRAIN;
        vec2 pos = vec2(wX, wY);
        pos += vec2((1.0f / WORLD_TO_TERRAIN) * 0.5f);

        vector_const<InverseInfluence, TerrainBubble::MAX + TerrainCurve::MAX> inverseInfluences;
        bool onPoint = false;
        for (int i = 0; i < bubbles.size(); i++) {
            if (!(affectMap[x][y] & 1UL << i)) 
                continue;

            TerrainInfluence influence = bubbles[i].GetInfluence(pos);
            if (influence.distance == 0.0f) {
                terrainMap[y][x].y = influence.height;
                onPoint = true;
                break;
            }
            else if (influence.distance <= 1.0f) {
                float inverseDistance = 1.0f / (influence.distance * influence.distance);
                inverseDistance -= 1.0f;
                inverseInfluences.push_back({inverseDistance, influence.height});
            }
        }
        if (onPoint)
            continue;

        for (int i = 0; i < curves.size(); i++) {
            if (!(affectMap[x][y] & 1UL << (i + TerrainBubble::MAX))) 
                continue;

            TerrainInfluence influence = curves[i].GetInfluence(pos);
            if (influence.distance == 0.0f) {
                terrainMap[y][x].y = influence.height;
                onPoint = true;
                break;
            }
            else if (influence.distance <= 1.0f) {
                float inverseDistance = 1.0f / (influence.distance * influence.distance);
                inverseDistance -= 1.0f;
                inverseInfluences.push_back({inverseDistance, influence.height});
            }
        }
        if (onPoint)
            continue;

        float totalInverseDistances = 0.0f;
        for (int i = 0; i < inverseInfluences.size(); i++)
            totalInverseDistances += inverseInfluences[i].inverseWeight;
        if (totalInverseDistances == 0.0f)
            continue;

        for (int i = 0; i < inverseInfluences.size(); i++)
            terrainMap[y][x].y += (inverseInfluences[i].inverseWeight / totalInverseDistances) * inverseInfluences[i].height;
    }}
}

template <const int RES>
void BaseGenerateTerrainHeights(
    glm::vec2 terrainMap[RES][RES],
    uint32_t affectMap[RES][RES],
    vector_contig<TerrainBubble, TerrainBubble::MAX>& bubbles,
    vector_contig<TerrainCurve, TerrainCurve::MAX>& curves
) {
    for (int x = 0; x < RES; x++) {
    for (int y = 0; y < RES; y++) {
        affectMap[x][y] = 0;
    }}

    for (int i = 0; i < bubbles.size(); i++) 
        bubbles[i].WriteAffect<RES>(affectMap, i);
    for (int i = 0; i < curves.size(); i++) 
        curves[i].WriteAffect<RES>(affectMap, i + TerrainBubble::MAX);

    std::vector<std::thread> threads;
    int cores = std::thread::hardware_concurrency();
    int sectionSize = RES / cores;

    for (int i = 0; i < cores; i++) {
        int add = (i == cores - 1 && RES % cores != 0) ? 1 : 0;
        threads.push_back(std::thread([terrainMap, bubbles, curves, affectMap, sectionSize, i, add] {
            BaseGenerateTerrainHeightsSection<RES>(
                ivec2(i * sectionSize, 0),
                ivec2((i + 1) * sectionSize + add, RES),
                terrainMap,
                affectMap,
                bubbles,
                curves
            );
        }));
    }
    for (std::thread& thread : threads)
        thread.join();
}

void Terrain::GenerateTerrainHeights(bool lowRes, EntityList* entities) {
    vector_const<int, 128> groundedEntities;
    if (entities != nullptr) {
        for (int i = 0; i < 128; i++) {
            Entity& entity = (*entities)[i];
            if (!entity.alive_) continue;

            if (abs(GetHeight(entity.transform_.position) - entity.transform_.position.y) < 5.0f)
                groundedEntities.push_back(i);
        }
    }

    if (lowRes) {
        BaseGenerateTerrainHeights<RESOLUTION_LOW>(
            terrainMapLow_, 
            affectMapLow_, 
            bubbles_, 
            curves_
        );
        lowRes_ = true;
        resourceManager_.UpdateTerrainMapTextureLow((glm::vec2*)terrainMapLow_);
    }
    else {
        BaseGenerateTerrainHeights<RESOLUTION>(
            terrainMap_, 
            affectMap_, 
            bubbles_, 
            curves_
        );
        resourceManager_.UpdateTerrainMapTexture((glm::vec2*)terrainMap_);
        lowRes_ = false;

        area_ = 0;
        for (int x = 0; x < RESOLUTION; x++) {
        for (int y = 0; y < RESOLUTION; y++) {
            if (terrainMap_[y][x].x <= 0.0f)
                area_++;
        }}
    }

    for (int entityIndex : groundedEntities) {
        Entity& entity = (*entities)[entityIndex];
        entity.transform_.position.y = GetHeight(entity.transform_.position);
    }
}

glm::vec2 Terrain::SampleTerrainMap(float x, float y, TerrainAccuracy accuracy) const {
    if (lowRes_) {
        x *= WORLD_TO_TERRAIN_SCALAR_LOW;
        x += HALF_RESOLUTION_LOW;
        y *= WORLD_TO_TERRAIN_SCALAR_LOW;
        y += HALF_RESOLUTION_LOW;

        int sX = (int)x % RESOLUTION_LOW;
        int sY = (int)y % RESOLUTION_LOW;
        return terrainMapLow_[sY][sX];
    }

    switch (accuracy) {
        case TA_Normal: {
            x *= WORLD_TO_TERRAIN_SCALAR;
            x += HALF_RESOLUTION;
            y *= WORLD_TO_TERRAIN_SCALAR;
            y += HALF_RESOLUTION;

            int sX = std::clamp((int)x, 0, RESOLUTION);
            int sY = std::clamp((int)y, 0, RESOLUTION);
            int sX1 = std::min(sX + 1, RESOLUTION);
            int sY1 = std::min(sY + 1, RESOLUTION);

            float a = x - sX;
            float b = y - sY;

            return 
                (1 - b) * (1 - a) * terrainMap_[sY][sX] +
                b * (1 - a) * terrainMap_[sY1][sX] +
                (1 - b) * a * terrainMap_[sY][sX1] +
                b * a * terrainMap_[sY1][sX1];
        }

        case TA_Low: {
            x *= WORLD_TO_TERRAIN_SCALAR;
            x += HALF_RESOLUTION;
            y *= WORLD_TO_TERRAIN_SCALAR;
            y += HALF_RESOLUTION;

            int sX = (int)x % RESOLUTION;
            int sY = (int)y % RESOLUTION;
            return terrainMap_[sY][sX];
        }
        
        default:
            return vec2(0.0f, 0.0f);
    }
}

glm::vec2 Terrain::SampleTerrainMap(const glm::vec2& position, TerrainAccuracy accuracy) const {
    return SampleTerrainMap(position.x, position.y, accuracy);
}

vec2 Terrain::GetDistance(const vec2& position, TerrainAccuracy accuracy) const {
    return getTerrainDistance(position, *this, accuracy);
}

vec2 Terrain::GetDistance(const vec3& position, TerrainAccuracy accuracy) const {
    return GetDistance(vec2(position.x, position.z), accuracy);
}

float Terrain::GetHeight(const vec2& position, TerrainAccuracy accuracy) const {
    vec2 worldDistance = getTerrainDistance(
        position,
        *this,
        accuracy
    );

    if (worldDistance.x > 0.0f)
        return -INFINITY;

    return worldDistance.y;
}

float Terrain::GetHeight(const vec3& position, TerrainAccuracy accuracy) const {
    float height = GetHeight(vec2(position.x, position.z), accuracy);
    return height;
}

vec3 Terrain::GetNormal(const vec2& position, TerrainAccuracy accuracy) const {
    return getTerrainNormal(position, *this, accuracy);
}

vec3 Terrain::GetNormal(const vec3& position, TerrainAccuracy accuracy) const {
    return GetNormal(vec2(position.x, position.z), accuracy);
}
