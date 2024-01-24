#include "Terrain.h"
#include "Shared_TerrainFuncs.h"
#include "Logging/Logger.h"
#include "Level/LevelProperties.h"
#include "Resource/ResourceManager.h"
#include "Entity/EntityList.h"
#include "Helpers/Ease.h"
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
    highResDirty_ = true;

    bubbles_.clear();
    curves_.clear();
    for (int x = 0; x < RESOLUTION; x++) {
    for (int y = 0; y < RESOLUTION; y++) {
        terrainMap_[x][y] = vec2(0.0f);
    } }

    nodeModel_ = resourceManager.GetModel("st_default");
    bubbleMaterial_.shader = resourceManager.GetShader("vs_static", "fs_color");
    bubbleMaterial_.properties.color = vec4(0.0f, 1.0f, 0.0f, 0.5f);
    curveMaterial_.shader = resourceManager.GetShader("vs_static", "fs_color");
    curveMaterial_.properties.color = vec4(0.0f, 0.0f, 1.0f, 0.5f);
    curveControlMaterial_.shader = resourceManager.GetShader("vs_static", "fs_color_front");
    curveControlMaterial_.properties.color = vec4(0.5f, 0.0f, 1.0f, 0.5f);

    bubbles_.push_back({vec4(0.0f, 12.0f, 25.0f, 100.0f)});
    bubbles_.push_back({vec4(50.0f, 16.0f, 50.0f, 150.0f)});
    bubbles_.push_back({vec4(-100.0f, 18.0f, -150.0f, 175.0f)});
    bubbles_.push_back({vec4(-75.0f, 6.0f, -0.0f, 125.0f)});

    TerrainCurve curve;
    curve.points[0] = vec4(-50.0f, -12.0f, -50.0f, 80.0f);
    curve.points[1] = vec4(0.0f, -18.0f, 50.0f, 20.0f);
    curve.points[2] = vec4(50.0f, -14.0f, 50.0f, 20.0f);
    curve.points[3] = vec4(110.0f, -12.0f, -60.0f, 80.0f);
    curves_.push_back(curve);
}

struct InverseInfluence {
    float inverseWeight;
    float height;
};

template <const int RES>
void BaseGenerateTerrainMapSection(
    const glm::vec2& start,
    const glm::vec2& end,
    glm::vec2 terrainMap[RES][RES],
    uint32_t affectMap[RES][RES],
    const vector_contig<TerrainBubble, TerrainBubble::MAX>& bubbles,
    const vector_contig<TerrainCurve, TerrainCurve::MAX>& curves
) {
    const int HALF_RES = RES * 0.5f;
    const float WORLD_TO_TERRAIN = RES / RANGE;

    FastNoiseLite noise;
    for (int x = 0; x < RES; x++) {
    for (int y = 0; y < RES; y++) {
        vec2 pos = vec2(x - HALF_RES + 0.5f, y - HALF_RES + 0.5f);
        vec2 samplePos = normalize(pos);
        samplePos *= 150;
        float noiseVal = noise.GetNoise(samplePos.x, samplePos.y);
        noiseVal = (noiseVal + 1.0f) * 0.5f;

        float blobRadius = lerp(150.0f, 200.0f, noiseVal);
        float curRadius = length(pos) / WORLD_TO_TERRAIN;

        terrainMap[y][x].x = curRadius - blobRadius;
    } }

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
                float inverseDistance = 1.0f / EaseInOutCubic(influence.distance);
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
                float inverseDistance = 1.0f / EaseInOutCubic(influence.distance);
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
void BaseGenerateTerrainMap(
    EntityList* entities,
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
            BaseGenerateTerrainMapSection<RES>(
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

void Terrain::GenerateTerrainMap(bool lowRes, EntityList* entities) {
    vector_const<int, 128> groundedEntities;
    for (int i = 0; i < 128; i++) {
        Entity& entity = (*entities)[i];
        if (!entity.alive_) continue;

        if (abs(GetHeight(entity.transform_.position) - entity.transform_.position.y) < 5.0f)
            groundedEntities.push_back(i);
    }

    if (lowRes) {
        BaseGenerateTerrainMap<RESOLUTION_LOW>(
            entities, 
            terrainMapLow_, 
            affectMapLow_, 
            bubbles_, 
            curves_
        );
        lowRes_ = true;
        highResDirty_ = true;
        resourceManager_.UpdateTerrainMapTextureLow((glm::vec2*)terrainMapLow_);
    }
    else {
        BaseGenerateTerrainMap<RESOLUTION>(
            entities, 
            terrainMap_, 
            affectMap_, 
            bubbles_, 
            curves_
        );
        resourceManager_.UpdateTerrainMapTexture((glm::vec2*)terrainMap_);
        lowRes_ = false;
        highResDirty_ = false;

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

            int sX = (int)x % RESOLUTION;//std::clamp((int)x, 0, RESOLUTION);
            int sY = (int)y % RESOLUTION;//std::clamp((int)y, 0, RESOLUTION);
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
    // If the position is below 10 units of the ground,
    // count it as un grounded
    // if (position.y < height - 10.0f)
    //     return -INFINITY;

    return height;
}

vec3 Terrain::GetNormal(const vec2& position, TerrainAccuracy accuracy) const {
    return getTerrainNormal(position, *this, accuracy);
}

vec3 Terrain::GetNormal(const vec3& position, TerrainAccuracy accuracy) const {
    return GetNormal(vec2(position.x, position.z), accuracy);
}
