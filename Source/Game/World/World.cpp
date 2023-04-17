#include "World.h"
#include "../../Helpers/ChunkHelpers.h"
#include "../../Logging/Logger.h"
#include "../../Logging/ScreenText.h"

using namespace glm;

World::World(ResourceManager& resourceManager):
    noise_(new FastNoiseLite()),
    resourceManager_(resourceManager)
{
    GenerateNoiseTexture_P();
    for (int x = -MAX_X_CHUNKS / 2; x < MAX_X_CHUNKS / 2; x++)
    for (int y = -MAX_Y_CHUNKS / 2; y < MAX_Y_CHUNKS / 2; y++)
    for (int z = -MAX_Z_CHUNKS / 2; z < MAX_Z_CHUNKS / 2; z++) {
        ivec3 chunk(x, y, z);
        dirtyChunks_.insert(chunk);
    }
}

float World::GetTerrainHeight(vec2 position) const {
    float height = noise_->GetNoise(position.x * 0.75f, position.y * 0.75f) * 8.0f + 8.0f;
    height += 32.0f;
    for (uint32_t i = 0; i < terrainModifiers_.GetCount(); i++) {
        const TerrainModifier& modifier = terrainModifiers_[i];
        float distFromModifier = distance(modifier.position, position);
        // Can probably precompute the chunk for faster performance
        if (distFromModifier > modifier.range)
            continue;

        float t = (modifier.range - distFromModifier) / modifier.range;
        float ease = 
            t < 0.5f ? 
            powf(2.0f, modifier.exponent - 1.0f) * powf(t , modifier.exponent) : 
            1 - powf(-2 * t + 2, modifier.exponent) / 2.0f; 

        height += ease * modifier.height;
    };

    return height;
}

float World::GetDistance(vec3 position) const {
    // Sample perlin noise in a circle following the position to form a blob
    vec2 position2d = vec2(position.x, position.z);

    vec2 noiseDir = normalize(position2d) * 64.0f;
    float blobRadius = 0.0f;
    if (length(noiseDir) > 0.0f)
        blobRadius = noise_->GetNoise(noiseDir.x, noiseDir.y) * 32.0f;

    float radius = 160.0f + blobRadius;

    float height = GetTerrainHeight(position2d);

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

void World::AddTerrainModifier(TerrainModifier modifier) {
    terrainModifiers_.Append(modifier);
    ivec2 origin = GetChunkAtWorldPosition2D(modifier.position); 
    int radius = int(ceilf(modifier.range / CHUNK_SIZE));
    for (int x = -radius; x <= radius; x++)
    for (int z = -radius; z <= radius; z++)
    for (int y = -MAX_Y_CHUNKS / 2; y < MAX_Y_CHUNKS / 2; y++) {
        ivec3 chunkToFlag = ivec3(origin.x + x, y, origin.y + z);
        dirtyChunks_.insert(chunkToFlag);
    }
}

void World::UpdateDirtyChunks() {
    UpdateModifiersGPU_P();

    int updates = 0;
    auto it = dirtyChunks_.begin();
    while (it != dirtyChunks_.end() && updates < 16) {
        ivec3 dirtyChunk = *it;
        GenerateMeshGPU_P(dirtyChunk);
        it = dirtyChunks_.erase(it);
        updates++;
    }
}
