#include "Terrain.h"
#include "Shared_TerrainFuncs.h"
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
    noiseLayers_[0].active_ = true;
    noiseLayers_[0].multiplier_ = 1.0f;

    noiseLayers_[1].active_ = true;
    noiseLayers_[1].seed_ = 50124;
    noiseLayers_[1].frequency_ = 0.1f;
    noiseLayers_[1].multiplier_ = 2.0f;
}

void Terrain::GenerateHeightmap() {
    for (int x = 0; x < RESOLUTION; x++) 
    for (int y = 0; y < RESOLUTION; y++)  {
        heightmap_[y][x] = 0.0f;

        for (NoiseLayer& layer : noiseLayers_) {
            if (!layer.active_) continue;

            FastNoiseLite noise(layer.seed_);
            float layerVal = noise.GetNoise(
                (float)x * layer.frequency_, 
                (float)y * layer.frequency_
            );
            layerVal *= layer.multiplier_;
            layerVal = std::pow(layerVal, layer.exponent_);

            heightmap_[y][x] += layerVal;
        }
    };
}

float Terrain::SampleHeightmap(float x, float y, TerrainAccuracy accuracy) const {
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
                (1 - b) * (1 - a) * heightmap_[sY][sX] +
                b * (1 - a) * heightmap_[sY1][sX] +
                (1 - b) * a * heightmap_[sY][sX1] +
                b * a * heightmap_[sY1][sX1];
        }

        case TA_Low: {
            x *= WORLD_TO_TERRAIN_SCALAR;
            x += HALF_RESOLUTION;
            y *= WORLD_TO_TERRAIN_SCALAR;
            y += HALF_RESOLUTION;

            int sX = (int)x % RESOLUTION;//std::clamp((int)x, 0, RESOLUTION);
            int sY = (int)y % RESOLUTION;//std::clamp((int)y, 0, RESOLUTION);
            return heightmap_[sY][sX];
        }
        
        default:
            return 0.0f;
    }
}

float Terrain::SampleHeightmap(const glm::vec2& position, TerrainAccuracy accuracy) const {
    return SampleHeightmap(position.x, position.y, accuracy);
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

    if (worldDistance.x > 32.0f)
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
