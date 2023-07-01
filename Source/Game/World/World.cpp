#include "World.h"
#include <FastNoiseLite.h>
using namespace glm;

World::World(FastNoiseLite& noise):
    noise_(noise)
{

}

float World::GetHeight(vec2 position) const {
    float height = noise_.GetNoise(position.x * 0.75f, position.y * 0.75f);
    height = (height + 1.0f) * 0.5f * 12.0f;
    return height;
}

float World::GetHeight(vec3 position) const {
    return GetHeight(vec2(position.x, position.z));
}

vec3 World::GetNormal(vec2 position, float epsilon) const {
    vec2 dX = position - vec2(1.0f, 0.0f);
    vec2 dZ = position - vec2(0.0f, 1.0f);

    float height = GetHeight(position);
    float gradX = GetHeight(dX) - height;
    float gradZ = GetHeight(dZ) - height;

    return normalize(vec3(gradX, 1.0f, gradZ));
}

vec3 World::GetNormal(vec3 position, float epsilon) const {
    return GetNormal(vec2(position.x, position.z), epsilon);
}
