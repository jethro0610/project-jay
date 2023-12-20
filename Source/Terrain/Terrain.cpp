#include "Terrain.h"
#include "Shared_TerrainFuncs.h"
#include "Logging/Logger.h"
#include <FastNoiseLite.h>

using namespace glm;

Terrain::Terrain():
    properties_({
        0.0f,
        128.0f,
        256.0f,
        128.0f,
        0.075f,
        2.0f,
        *this
    })
{
    blobProperties_.seed_ = 1337;
    blobProperties_.frequency_ = 100.0f;
    blobProperties_.maxRadius_ = 150.0f;
    blobProperties_.minRadius_ = 150.0f;

    GenerateTerrainMap();
}

void Terrain::GenerateTerrainMap() {
    FastNoiseLite blobNoise(blobProperties_.seed_);
    for (int x = 0; x < RESOLUTION; x++) 
    for (int y = 0; y < RESOLUTION; y++)  {
        vec2 pos = vec2(x - HALF_RESOLUTION, y - HALF_RESOLUTION);
        vec2 samplePos = normalize(pos);
        samplePos *= blobProperties_.frequency_;
        float noiseVal = blobNoise.GetNoise(samplePos.x, samplePos.y);
        noiseVal = (noiseVal + 1.0f) * 0.5f;

        float blobRadius = lerp(blobProperties_.minRadius_, blobProperties_.maxRadius_, noiseVal);
        float curRadius = length(pos) / WORLD_TO_TERRAIN_SCALAR;

        terrainMap_[y][x].x = curRadius - blobRadius;
    };

    std::array<FastNoiseLite, 4> noises;
    for (int i = 0; i < 4; i++)
        noises[i].SetSeed(noiseLayers_[i].seed_);

    for (int x = 0; x < RESOLUTION; x++) 
    for (int y = 0; y < RESOLUTION; y++)  {
        terrainMap_[y][x].y = 0.0f;

        for (int i = 0; i < 4; i++) {
            const NoiseLayer& layer = noiseLayers_[i];
            if (!layer.active_) continue;

            float layerVal = noises[i].GetNoise(
                (float)x * layer.frequency_, 
                (float)y * layer.frequency_
            );
            layerVal *= layer.multiplier_;
            layerVal = std::pow(layerVal, layer.exponent_);

            terrainMap_[y][x].y += layerVal;
        }

        float edgeDistance = max((terrainMap_[y][x].x + 24.0f) * 0.05f, 0.0f);
        terrainMap_[y][x].y += min(0.0f, -pow(edgeDistance, 2.0f));
    }
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
    return getTerrainDistance(position, properties_, accuracy);
}

vec2 Terrain::GetDistance(const vec3& position, TerrainAccuracy accuracy) const {
    return GetDistance(vec2(position.x, position.z), accuracy);
}

float Terrain::GetHeight(const vec2& position, TerrainAccuracy accuracy) const {
    vec2 worldDistance = getTerrainDistance(
        position,
        properties_,
        accuracy
    );

    if (worldDistance.x > 0.0f)
        return -INFINITY;

    return worldDistance.y;
}

float Terrain::GetHeight(const vec3& position, TerrainAccuracy accuracy) const {
    float height = GetHeight(vec2(position.x, position.z), accuracy);
    if (position.y < height - 1.0f)
        return -INFINITY;

    return height;
}

vec3 Terrain::GetNormal(const vec2& position, TerrainAccuracy accuracy) const {
    return getTerrainNormal(position, properties_, accuracy);
}

vec3 Terrain::GetNormal(const vec3& position, TerrainAccuracy accuracy) const {
    return GetNormal(vec2(position.x, position.z), accuracy);
}
