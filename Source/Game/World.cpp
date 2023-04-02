#include "World.h"
#include <cmath>
#include "../Logging/Logger.h"
#include <gtx/string_cast.hpp>
using namespace glm;

World::World(Entity* entities, ResourceManager& resourceManager, TerrainModComponent& terrainModComponent):
    noise_(new FastNoiseLite()),
    entities_(entities),
    resourceManager_(resourceManager),
    terrainModComponent_(terrainModComponent)
{
    GenerateNoiseTexture_P();
}

float World::GetDistance(vec3 position) const {
    // Sample perlin noise in a circle following the position to form a blob
    vec2 position2d = vec2(position.x, position.z);

    vec2 noiseDir = normalize(position2d) * 64.0f;
    float blobRadius = 0.0f;
    if (length(noiseDir) > 0.0f)
        blobRadius = noise_->GetNoise(noiseDir.x, noiseDir.y) * 32.0f;

    float radius = 160.0f + blobRadius;

    float noiseHeight = noise_->GetNoise(position2d.x * 0.75f, position2d.y * 0.75f) * 8.0f + 8.0f;
    float height = 32.0f + noiseHeight;

    vec2 d = vec2(length(position2d), abs(position.y)) - vec2(radius, height) + height; 
    float blobDist = length(max(d, 0.0f)) + min(max(d.x, d.y), 0.0f) - height;

    return blobDist;
}

vec3 World::GetNormal(vec3 position, float epsilon) const {
    vec3 dX = position + vec3(epsilon, 0.0f, 0.0f);
    vec3 dY = position + vec3(0.0f, epsilon, 0.0f);
    vec3 dZ = position + vec3(0.0f, 0.0f, epsilon);

    float gradX = GetDistance(dX);
    float gradY = GetDistance(dY);
    float gradZ = GetDistance(dZ);
    return normalize(vec3(gradX, gradY, gradZ));
}

float World::Lerp(float a, float b, float t) {
    return a + t * (b - a);
}

void World::MarkChunkDirty(ivec3 chunk) {
    int index = (chunk.x) + (chunk.y * MAX_X_CHUNKS) + (chunk.z * MAX_X_CHUNKS * MAX_Y_CHUNKS); 
    dirtyChunks_[index] = true;
}

bool World::ChunkIsDirty(ivec3 chunk) const {
    int index = (chunk.x) + (chunk.y * MAX_X_CHUNKS) + (chunk.z * MAX_Y_CHUNKS * MAX_Z_CHUNKS); 
    return dirtyChunks_[index];
}

vec3 World::GetNearestInDirection(vec3 start, vec3 direction, uint16_t maxSteps) {
    vec3 currentPosition = start;
    for (int i = 0; i < maxSteps; i++) {
        float distance = GetDistance(currentPosition);
        if (abs(distance) < 0.001f)
            break;
        currentPosition += direction * distance;
    }
    return currentPosition;
}
