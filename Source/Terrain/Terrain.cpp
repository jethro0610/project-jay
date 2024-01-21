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

void Terrain::GenerateTerrainMap(
    const BlobProperties& blob
) {
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
    } }

    vector_contig<TerrainBubble, 4> bubbles;
    bubbles.push_back({vec3(0.0f, 12.0f, 25.0f), 100.0f});
    bubbles.push_back({vec3(50.0f, 16.0f, 50.0f), 150.0f});
    bubbles.push_back({vec3(-100.0f, 18.0f, -150.0f), 175.0f});

    for (int x = 0; x < RESOLUTION; x++) {
    for (int y = 0; y < RESOLUTION; y++) {
        terrainMap_[y][x].y = 0.0f;
        float wX = x - HALF_RESOLUTION;
        wX /= WORLD_TO_TERRAIN_SCALAR;
        float wY = y - HALF_RESOLUTION;
        wY /= WORLD_TO_TERRAIN_SCALAR;
        vec2 pos = vec2(wX, wY);

        vector_const<float, 4> inverseDistances;
        vector_const<float, 4> distances;
        for (int i = 0; i < bubbles.size(); i++) {
            vec2 bubblePos = vec2(bubbles[i].position.x, bubbles[i].position.z);
            float d = distance(pos, bubblePos) / bubbles[i].radius;
            distances.push_back(d);

            if (d > 1.0f) {
                inverseDistances.push_back(0.0f);
                continue;
            }

            if (d == 0.0f)
                d = INFINITY;
            else
                d = std::pow(d, -2.0f);
            d -= 1.0f;

            inverseDistances.push_back(d);
        }

        float totalInverseDistances = 0.0f;
        for (int i = 0; i < inverseDistances.size(); i++) {
            totalInverseDistances += inverseDistances[i];
        }

        float val = 0.0f;
        for (int i = 0; i < bubbles.size(); i++) {
            if (inverseDistances[i] <= 0.0f)
                continue;

            if (distances[i] == 0.0f) {
                val = bubbles[i].position.y;
                break;
            }

            float t = EaseInOutQuad(distances[i]);
            float height = bubbles[i].position.y * (1.0f - t);
            val += (inverseDistances[i] / totalInverseDistances) * height;
        }
        terrainMap_[y][x].y = val;
        float edgeDistance = max((terrainMap_[y][x].x + EDGE_OFFSET) * 0.1f, 0.0f);
        terrainMap_[y][x].y += min(0.0f, -pow(edgeDistance, 2.0f));
    } }

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
