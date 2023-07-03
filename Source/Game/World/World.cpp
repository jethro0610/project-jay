#include "World.h"
#include <FastNoiseLite.h>
#include "Shared_WorldFuncs.h"

using namespace glm;

const float END_HEIGHT = -16.0f;

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

float World::GetHeight(vec2 position) const {
    vec2 worldDistance = getWorldDistance(
        position,
        properties_ 
    );

    if (worldDistance.x < -32.0f)
        return -INFINITY;

    return worldDistance.y;
}

float World::GetHeight(vec3 position) const {
    return GetHeight(vec2(position.x, position.z));
}

vec3 World::GetNormal(vec2 position) const {
    return getWorldNormal(position, properties_);
}

vec3 World::GetNormal(vec3 position) const {
    return GetNormal(vec2(position.x, position.z));
}
