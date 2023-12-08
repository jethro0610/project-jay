#include "Terrain.h"
#include <FastNoiseLite.h>
#include "Shared_TerrainFuncs.h"

using namespace glm;

Terrain::Terrain(Noise& noise):
    noise_(noise),
    properties_({
        0.0f,
        128.0f,
        256.0f,
        128.0f,
        0.075f,
        2.0f,
        noise
    })
{

}

vec2 Terrain::GetDistance(const vec2& position, NoiseAccuracy accuracy) const {
    return getTerrainDistance(position, properties_, accuracy);
}

vec2 Terrain::GetDistance(const vec3& position, NoiseAccuracy accuracy) const {
    return GetDistance(vec2(position.x, position.z), accuracy);
}

float Terrain::GetHeight(const vec2& position, NoiseAccuracy accuracy) const {
    vec2 worldDistance = getTerrainDistance(
        position,
        properties_,
        accuracy
    );

    if (worldDistance.x > 32.0f)
        return -INFINITY;

    return worldDistance.y;
}

float Terrain::GetHeight(const vec3& position, NoiseAccuracy accuracy) const {
    float height = GetHeight(vec2(position.x, position.z), accuracy);
    if (position.y < height - 1.0f)
        return -INFINITY;

    return height;
}

vec3 Terrain::GetNormal(const vec2& position, NoiseAccuracy accuracy) const {
    return getTerrainNormal(position, properties_, accuracy);
}

vec3 Terrain::GetNormal(const vec3& position, NoiseAccuracy accuracy) const {
    return GetNormal(vec2(position.x, position.z), accuracy);
}
