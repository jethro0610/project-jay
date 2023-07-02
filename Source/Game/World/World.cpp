#include "World.h"
#include <FastNoiseLite.h>
#include "WorldShared.h"

using namespace glm;

World::World(FastNoiseLite& noise):
    noise_(noise)
{
    minRadius_ = 128.0f;
    maxRadius_ = 256.0f;
    edgeJaggedness_ = 128.0f;
    edgeFalloff_ = 0.075f;
    edgePower_ = 2.0f;
}

float World::GetHeight(vec2 position) const {
    WorldProperties props = { 
        0.0f, 
        minRadius_, 
        maxRadius_, 
        edgeJaggedness_, 
        edgeFalloff_, 
        edgePower_, 
        noise_ 
    };
    return getHeight(
        position,
        props
    );
}

float World::GetHeight(vec3 position) const {
    return GetHeight(vec2(position.x, position.z));
}

vec3 World::GetNormal(vec2 position) const {
    WorldProperties props = { 
        0.0f, 
        minRadius_, 
        maxRadius_, 
        edgeJaggedness_, 
        edgeFalloff_, 
        edgePower_, 
        noise_ 
    };

    return getNormal(position, props);
}

vec3 World::GetNormal(vec3 position) const {
    return GetNormal(vec2(position.x, position.z));
}
