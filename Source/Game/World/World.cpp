#include "World.h"
#include "../../Helpers/ChunkHelpers.h"
using namespace glm;

World::World():
    noise_(new FastNoiseLite())
{
    GenerateNoiseTexture_P();
    MarkAllDirty();
}

float World::GetHeight(vec2 position) const {
    float height = noise_->GetNoise(position.x * 0.75f, position.y * 0.75f) * 8.0f + 8.0f;
    return height + 32.0f;
}

float World::GetHeight(vec3 position) const {
    return GetHeight(vec2(position.x, position.z));
}

vec3 World::GetNormal(vec2 position, float epsilon) const {
    vec2 dX = position + vec2(epsilon, 0.0f);
    vec2 dZ = position + vec2(0.0f, epsilon);

    float gradX = GetHeight(dX);
    float gradZ = GetHeight(dZ);
    return normalize(vec3(gradX, 1.0f, gradZ));
}

void World::UpdateDirtyChunks() {
    int updates = 0;
    auto it = dirtyChunks_.begin();
    while (it != dirtyChunks_.end() && updates < 12) {
        ivec3 dirtyChunk = *it;
        GenerateMeshGPU_P(dirtyChunk);
        it = dirtyChunks_.erase(it);
        updates++;
    }
}

void World::MarkAllDirty() {
    for (int x = -MAX_X_CHUNKS / 2; x < MAX_X_CHUNKS / 2; x++)
    for (int y = -MAX_Y_CHUNKS / 2; y < MAX_Y_CHUNKS / 2; y++)
    for (int z = -MAX_Z_CHUNKS / 2; z < MAX_Z_CHUNKS / 2; z++) {
        ivec3 chunk(x, y, z);
        dirtyChunks_.insert(chunk);
    }
}
