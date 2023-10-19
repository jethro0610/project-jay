#include "World.h"
#include <FastNoiseLite.h>
#include "Shared_WorldFuncs.h"

using namespace glm;

World::World(Noise& noise):
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

vec2 World::GetDistance(const vec2& position, NoiseAccuracy accuracy) const {
    return getWorldDistance(position, properties_, accuracy);
}

vec2 World::GetDistance(const vec3& position, NoiseAccuracy accuracy) const {
    return GetDistance(vec2(position.x, position.z), accuracy);
}

float World::GetHeight(const vec2& position, NoiseAccuracy accuracy) const {
    vec2 worldDistance = getWorldDistance(
        position,
        properties_,
        accuracy
    );

    if (worldDistance.x > 32.0f)
        return -INFINITY;

    return worldDistance.y;
}

float World::GetHeight(const vec3& position, NoiseAccuracy accuracy) const {
    float height = GetHeight(vec2(position.x, position.z), accuracy);
    if (position.y < height - 1.0f)
        return -INFINITY;

    return height;
}

vec3 World::GetNormal(const vec2& position, NoiseAccuracy accuracy) const {
    return getWorldNormal(position, properties_, accuracy);
}

vec3 World::GetNormal(const vec3& position, NoiseAccuracy accuracy) const {
    return GetNormal(vec2(position.x, position.z), accuracy);
}
