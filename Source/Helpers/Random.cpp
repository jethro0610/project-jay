#include <random>
#include "Random.h"
using namespace glm;

float RandomFloatRange(float min, float max) {
    if (min == max)
        return min;

    float range = max - min; 
    int modVal = range * 100;

    return ((rand() % modVal) * 0.01f) + min;
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

vec3 RandomVector(glm::vec3& min, glm::vec3& max) {
    float x = RandomFloatRange(min.x, max.x);
    float y = RandomFloatRange(min.y, max.y);
    float z = RandomFloatRange(min.z, max.z);

    return vec3(x, y, z);
}

vec4 RandomVec4(glm::vec4& min, glm::vec4& max) {
    float x = RandomFloatRange(min.x, max.x);
    float y = RandomFloatRange(min.y, max.y);
    float z = RandomFloatRange(min.z, max.z);
    float w = RandomFloatRange(min.w, max.w);

    return vec4(x, y, z, w);
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
