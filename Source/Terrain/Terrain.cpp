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

Terrain::Terrain(
    LevelProperties& levelProperties,
    ResourceManager& resourceManager
):
resourceManager_(resourceManager)
{
    for (int x = 0; x < RESOLUTION; x++) {
    for (int y = 0; y < RESOLUTION; y++) {
        terrainMap_[x][y] = vec2(0.0f);
    } }
}


struct TerrainBubble {
    vec3 position;
    float radius;
};

struct TerrainCurve {
    vec4 points[4];
};

inline vec4 GetCubicBezierPosition(TerrainCurve& curve, float t) {
    t = std::clamp(t, 0.0f, 1.0f);
    float invT = 1.0f - t;

    vec4 point = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    point += invT * invT * invT * curve.points[0];
    point += (t * 3.0f * invT * invT) * curve.points[1];
    point += (t * t * 3.0f * invT) * curve.points[2];
    point += t * t * t * curve.points[3];

    return point;
}

static constexpr int ITERATIONS = 12;

struct CurveInfluence {
    float strength;
    float height;
};

inline CurveInfluence GetCurveInfluence(TerrainCurve& curve, vec2 pos) {
    float pivot = 0.0f;
    float nearestSqr = INFINITY;
    float nearestRadius = 0.0f;
    float nearestHeight = 0.0f;
    float notchLength = 0.25f;

    for (int i = 1; i < 4; i++) {
        float t = i * notchLength;
        vec4 bezierPos = GetCubicBezierPosition(curve, t);
        float dSqr = distance2(vec2(bezierPos.x, bezierPos.z), pos);
        if (dSqr < nearestSqr) {
            nearestSqr = dSqr;
            pivot = t;
        }
    }

    for (int i = 0; i < ITERATIONS - 1; i++) {
        float start = pivot - notchLength;
        notchLength *= 0.5f;

        nearestSqr = INFINITY;
        for (int j = 0; j < 5; j++) {
            float t = start + j * notchLength;
            vec4 bezierPos = GetCubicBezierPosition(curve, t);
            float dSqr = distance2(vec2(bezierPos.x, bezierPos.z), pos);
            if (dSqr < nearestSqr) {
                nearestRadius = bezierPos.w;
                nearestHeight = bezierPos.y;
                nearestSqr = dSqr;
                pivot = std::clamp(t, 0.0f, 1.0f);
            }
        }
    }

    CurveInfluence influence;
    influence.strength = sqrt(nearestSqr) / nearestRadius;
    influence.height = nearestHeight;
    return influence;
}

void Terrain::GenerateTerrainMap(
    const BlobProperties& blob
) {
    area_ = 0.0;
    FastNoiseLite blobNoise(blob.seed);
    for (int x = 0; x < RESOLUTION; x++) {
    for (int y = 0; y < RESOLUTION; y++) {
        vec2 pos = vec2(x - HALF_RESOLUTION, y - HALF_RESOLUTION);
        vec2 samplePos = normalize(pos);
        samplePos *= blob.frequency;
        float noiseVal = blobNoise.GetNoise(samplePos.x, samplePos.y);
        noiseVal = (noiseVal + 1.0f) * 0.5f;

        float blobRadius = lerp(blob.minRadius, blob.maxRadius, noiseVal);
        float curRadius = length(pos) / WORLD_TO_TERRAIN_SCALAR;

        terrainMap_[y][x].x = curRadius - blobRadius;
        if (terrainMap_[y][x].x <= 0.0f)
            area_++;
    } }

    vector_contig<TerrainBubble, 4> bubbles;
    bubbles.push_back({vec3(0.0f, 12.0f, 25.0f), 100.0f});
    bubbles.push_back({vec3(50.0f, -4.0f, 50.0f), 150.0f});
    bubbles.push_back({vec3(-100.0f, 18.0f, -150.0f), 175.0f});
    bubbles.push_back({vec3(-75.0f, 6.0f, -0.0f), 125.0f});

    // for (int x = 0; x < RESOLUTION; x++) {
    // for (int y = 0; y < RESOLUTION; y++) {
    //     terrainMap_[y][x].y = 0.0f;
    //     float wX = x - HALF_RESOLUTION;
    //     wX /= WORLD_TO_TERRAIN_SCALAR;
    //     float wY = y - HALF_RESOLUTION;
    //     wY /= WORLD_TO_TERRAIN_SCALAR;
    //     vec2 pos = vec2(wX, wY);
    //
    //     vector_const<float, 4> inverseDistances;
    //     vector_const<float, 4> distances;
    //     for (int i = 0; i < bubbles.size(); i++) {
    //         vec2 bubblePos = vec2(bubbles[i].position.x, bubbles[i].position.z);
    //         float d = distance(pos, bubblePos) / bubbles[i].radius;
    //         distances.push_back(d);
    //
    //         if (d > 1.0f) {
    //             inverseDistances.push_back(0.0f);
    //             continue;
    //         }
    //
    //         if (d == 0.0f)
    //             d = INFINITY;
    //         else
    //             d = 1.0f / EaseInOutQuad(d);//std::pow(d, -0.25f);
    //         d -= 1.0f;
    //
    //         inverseDistances.push_back(d);
    //     }
    //
    //     float totalInverseDistances = 0.0f;
    //     for (int i = 0; i < inverseDistances.size(); i++) {
    //         totalInverseDistances += inverseDistances[i];
    //     }
    //
    //     float val = 0.0f;
    //     for (int i = 0; i < bubbles.size(); i++) {
    //         if (inverseDistances[i] <= 0.0f)
    //             continue;
    //
    //         if (distances[i] == 0.0f) {
    //             val = bubbles[i].position.y;
    //             break;
    //         }
    //
    //         float t = EaseInOutQuad(distances[i]);
    //         float height = bubbles[i].position.y * (1.0f - t);
    //         val += (inverseDistances[i] / totalInverseDistances) * height;
    //     }
    //     terrainMap_[y][x].y = val;
    //     float edgeDistance = max((terrainMap_[y][x].x + EDGE_OFFSET) * 0.1f, 0.0f);
    //     terrainMap_[y][x].y += min(0.0f, -pow(edgeDistance, 2.0f));
    // } }
    //


    TerrainCurve curve;
    curve.points[0] = vec4(-50.0f, -12.0f, -50.0f, 75.0f);
    curve.points[1] = vec4(0.0f, -18.0f, 50.0f, 20.0f);
    curve.points[2] = vec4(50.0f, -8.0f, 50.0f, 35.0f);
    curve.points[3] = vec4(100.0f, -4.0f, -50.0f, 75.0f);

    bool curveAffected[RESOLUTION][RESOLUTION];
    for (int x = 0; x < RESOLUTION; x++) {
    for (int y = 0; y < RESOLUTION; y++) {
        curveAffected[x][y] = false;
    }}

    for (int i = 0; i < 65; i++) {
        float t = i / 64.0f;
        vec4 bezierPos = GetCubicBezierPosition(curve, t);

        int mapX = bezierPos.x * WORLD_TO_TERRAIN_SCALAR;
        mapX += HALF_RESOLUTION;
        int mapY = bezierPos.z * WORLD_TO_TERRAIN_SCALAR;
        mapY += HALF_RESOLUTION;
        int range = (bezierPos.w + 10.0f) * WORLD_TO_TERRAIN_SCALAR;
        
        int startX = max(mapX - range, 0);
        int endX = min(mapX + range, RESOLUTION - 1);
        int startY = max(mapY - range, 0);
        int endY = min(mapY + range, RESOLUTION - 1);

        for (int x = startX; x <= endX; x++) {
        for (int y = startY; y <= endY; y++) {
            curveAffected[x][y] = true;
        }}
    }

    for (int x = 0; x < RESOLUTION; x++) {
    for (int y = 0; y < RESOLUTION; y++) {
        terrainMap_[y][x].y = 0.0f;
        float wX = x - HALF_RESOLUTION;
        wX /= WORLD_TO_TERRAIN_SCALAR;
        float wY = y - HALF_RESOLUTION;
        wY /= WORLD_TO_TERRAIN_SCALAR;
        vec2 pos = vec2(wX, wY);

        if (curveAffected[x][y]) {
            CurveInfluence influence = GetCurveInfluence(curve, pos);
            if (influence.strength <= 1.0f) {
                terrainMap_[y][x].y = EaseInOutQuad(1.0f - influence.strength) * influence.height;
            }
        }
    }}
    resourceManager_.UpdateTerrainMapTexture((glm::vec2*)terrainMap_);
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
