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
    float height = getWorldDistance(
        position,
        properties_ 
    ).y;

    if (height < properties_.minHeight + END_HEIGHT)
        return -INFINITY;

    return height;
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
