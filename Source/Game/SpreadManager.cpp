#include "SpreadManager.h"
using namespace glm;

SpreadManager::SpreadManager(ResourceManager* resourceManager, World* world) {
    resourceManager_ = resourceManager;
    world_ = world;
}

bool SpreadManager::AddSpread(ivec2 key, float height) {
    ivec2 chunkPos = (key * (int)SPREAD_DIST) / (int)CHUNK_SIZE;

    // Normalize the chunk position
    chunkPos.x += MAX_X_CHUNKS / 2;
    chunkPos.y += MAX_Z_CHUNKS / 2;
    SpreadChunk& chunk = chunks_[chunkPos.x][chunkPos.y]; 

    if (chunk.keys.contains(key))
        return false;
    assert(chunk.count != MAX_SPREAD);

    vec3 position = world_->GetNearestInDirection(vec3(key.x * SPREAD_DIST, height, key.y * SPREAD_DIST), -Transform::worldUp);
    chunk.positions[chunk.count] = position; 
    chunk.keys[key] = chunk.count;
    chunk.count++;

    dirtyChunks_.insert(chunkPos);

    return true;
}
