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
    cores_ = std::thread::hardware_concurrency();
    sectionSize_ = RESOLUTION / cores_;
    for (int i = 0; i < cores_; i++)
        completeThreads_[i] = true;

    threads_.clear();
    bubbles_.clear();
    curves_.clear();
    for (int x = 0; x < RESOLUTION; x++) {
    for (int y = 0; y < RESOLUTION; y++) {
        terrainMapBack_[x][y] = vec2(0.0f);
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

// void Terrain::GenerateTerrainMap(
//     const BlobProperties& blob
// ) {
//
// }

void Terrain::Update(const BlobProperties& blob) {
    bool doneGenerating = true;
    for (int i = 0; i < cores_; i++) {
        bool complete;
        threadMutexes_[i].lock();
        complete = completeThreads_[i];
        threadMutexes_[i].unlock();

        if (!complete) {
            doneGenerating = false;
            break;
        }
    }
    if (!doneGenerating)
        return;

    for (int i = 0; i < threads_.size(); i++)
        threads_[i].join();
    threads_.clear();

    for (int x = 0; x < RESOLUTION; x++) {
    for (int y = 0; y < RESOLUTION; y++) {
        affectMap_[x][y] = 0;
    }}

    for (int i = 0; i < bubbles_.size(); i++) 
        bubbles_[i].WriteAffect(affectMap_, i);
    for (int i = 0; i < curves_.size(); i++) 
        curves_[i].WriteAffect(affectMap_, i + TerrainBubble::MAX);
    bubblesBack_ = bubbles_;
    curvesBack_ = curves_;

    memcpy(terrainMap_, terrainMapBack_, sizeof(glm::vec2) * RESOLUTION *  RESOLUTION);
    resourceManager_.UpdateTerrainMapTexture((glm::vec2*)terrainMap_);

    int sectionSize = RESOLUTION / cores_;
    for (int i = 0; i < cores_; i++) {
        completeThreads_[i] = false;
        int add = (i == cores_ - 1 && RESOLUTION % cores_ != 0) ? 1 : 0;
        threads_.push_back(std::thread([this, sectionSize, i, add] {
            this->GenerateTerrainMapSectionThreaded(
                ivec2(i * sectionSize, 0),
                ivec2((i + 1) * sectionSize + add, RESOLUTION),
                i
            );
        }));
    }
}

void Terrain::GenerateTerrainMapSectionThreaded(
    const glm::ivec2& start,
    const glm::ivec2& end,
    int index
) {
    FastNoiseLite blobNoise(1337);
    for (int x = 0; x < RESOLUTION; x++) {
    for (int y = 0; y < RESOLUTION; y++) {
        vec2 pos = vec2(x - HALF_RESOLUTION, y - HALF_RESOLUTION);
        vec2 samplePos = normalize(pos);
        samplePos *= 150;
        float noiseVal = blobNoise.GetNoise(samplePos.x, samplePos.y);
        noiseVal = (noiseVal + 1.0f) * 0.5f;

        float blobRadius = lerp(150.0f, 250.0f, noiseVal);
        float curRadius = length(pos) / WORLD_TO_TERRAIN_SCALAR;

        terrainMapBack_[y][x].x = curRadius - blobRadius;
    } }

    for (int x = start.x; x < end.x; x++) {
    for (int y = start.y; y < end.y; y++) {
        terrainMapBack_[y][x].y = 0.0f;

        float wX = x - HALF_RESOLUTION;
        wX /= WORLD_TO_TERRAIN_SCALAR;
        float wY = y - HALF_RESOLUTION;
        wY /= WORLD_TO_TERRAIN_SCALAR;
        vec2 pos = vec2(wX, wY);

        vector_const<InverseInfluence, TerrainBubble::MAX + TerrainCurve::MAX> inverseInfluences;
        bool onPoint = false;
        for (int i = 0; i < bubblesBack_.size(); i++) {
            if (!(affectMap_[x][y] & 1UL << i)) 
                continue;

            TerrainInfluence influence = bubblesBack_[i].GetInfluence(pos);
            if (influence.distance == 0.0f) {
                terrainMapBack_[y][x].y = influence.height;
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

        for (int i = 0; i < curvesBack_.size(); i++) {
            if (!(affectMap_[x][y] & 1UL << (i + TerrainBubble::MAX))) 
                continue;

            TerrainInfluence influence = curvesBack_[i].GetInfluence(pos);
            if (influence.distance == 0.0f) {
                terrainMapBack_[y][x].y = influence.height;
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
            terrainMapBack_[y][x].y += (inverseInfluences[i].inverseWeight / totalInverseDistances) * inverseInfluences[i].height;
    }}

    threadMutexes_[index].lock();
    completeThreads_[index] = true;
    threadMutexes_[index].unlock();
}

glm::vec2 Terrain::SampleTerrainMap(float x, float y, TerrainAccuracy accuracy) const {
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
