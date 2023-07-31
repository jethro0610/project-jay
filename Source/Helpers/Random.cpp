#include <random>
#include "Random.h"
using namespace glm;

float RandomFloatRange(float min, float max) {
    float range = max - min; 
    int modVal = range * 1000;

    return ((rand() % modVal) * 0.001f) + min;
}

vec3 RandomVector(float maxDist) {
    float x = RandomFloatRange(-1.0f, 1.0f);
    float y = RandomFloatRange(-1.0f, 1.0f);
    float z = RandomFloatRange(-1.0f, 1.0f);

    vec3 direction = normalize(vec3(x, y, z));
    float dist = RandomFloatRange(0.0f, 1.0f);
    dist = sqrt(dist) * maxDist;

    return direction * dist;
}

vec2 RandomVector2D(float maxDist) {
    float x = RandomFloatRange(-1.0f, 1.0f);
    float z = RandomFloatRange(-1.0f, 1.0f);

    vec2 direction = normalize(vec2(x, z));
    float dist = RandomFloatRange(0.0f, 1.0f);
    dist = sqrt(dist) * maxDist;

    return direction * dist;
}

vec3 RandomVectorHeight(float maxDist, float minHeight, float maxHeight) {
    float x = RandomFloatRange(-1.0f, 1.0f);
    float y = RandomFloatRange(minHeight, maxHeight);
    float z = RandomFloatRange(-1.0f, 1.0f);

    vec2 direction = normalize(vec2(x, z));
    float dist = RandomFloatRange(0.0f, 1.0f);
    dist = dist * maxDist;
    direction *= dist;

    return vec3(direction.x, y, direction.y);
}
