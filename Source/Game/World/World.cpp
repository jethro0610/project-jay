#include "World.h"
#include <FastNoiseLite.h>
#include "Shared_WorldFuncs.h"

using namespace glm;

World::World(FastNoiseLite& noise):
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

vec2 World::GetDistance(vec2 position) const {
    return getWorldDistance(position, properties_);
}

vec2 World::GetDistance(vec3 position) const {
    return GetDistance(vec2(position.x, position.z));
}

float World::GetHeight(vec2 position) const {
    vec2 worldDistance = getWorldDistance(
        position,
        properties_ 
    );

    if (worldDistance.x > 32.0f)
        return -INFINITY;

    return worldDistance.y;
}

float World::GetHeight(vec3 position) const {
    float height = GetHeight(vec2(position.x, position.z));
    if (position.y < height - 1.0f)
        return -INFINITY;

    return height;
}

vec3 World::GetNormal(vec2 position) const {
    return getWorldNormal(position, properties_);
}

vec3 World::GetNormal(vec3 position) const {
    return GetNormal(vec2(position.x, position.z));
}
