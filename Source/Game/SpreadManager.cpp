#include "SpreadManager.h"
using namespace glm;

SpreadManager::SpreadManager(ResourceManager* resourceManager, World* world) {
    resourceManager_ = resourceManager;
    world_ = world;
}

ivec2 SpreadManager::WorldPositionToSpreadKey(vec3 position) const {
    vec2 spreadOrigin(position.x, position.z);
    spreadOrigin /= SPREAD_DIST;
    spreadOrigin = floor(spreadOrigin);
    return ivec2(spreadOrigin);
}

ivec2 SpreadManager::SpreadKeyToChunk(ivec2 key) const {
    ivec2 chunkPos = (key * (int)SPREAD_DIST) / (int)CHUNK_SIZE;

    // Normalize the chunk position
    chunkPos.x += MAX_X_CHUNKS / 2;
    chunkPos.y += MAX_Z_CHUNKS / 2;
    return chunkPos;
}

bool SpreadManager::SpreadIsActive(ivec2 key) const {
    ivec2 chunkPos = SpreadKeyToChunk(key); 
    const SpreadChunk& chunk = chunks_[chunkPos.x][chunkPos.y]; 
    return chunk.keys.contains(key);
}

bool SpreadManager::AddSpread(ivec2 key, float height) {
    ivec2 chunkPos = SpreadKeyToChunk(key); 
    SpreadChunk& chunk = chunks_[chunkPos.x][chunkPos.y]; 

    if (chunk.keys.contains(key))
        return false;
    assert(chunk.count != MAX_SPREAD);

    const float offset = SPREAD_DIST / 2.0f;
    vec3 position = vec3(key.x * SPREAD_DIST + offset, height, key.y * SPREAD_DIST + offset);
    position = world_->GetNearestInDirection(position, -Transform::worldUp);
    chunk.positions[chunk.count] = position; 
    chunk.keys[key] = chunk.count;
    chunk.count++;

    dirtyChunks_.insert(chunkPos);

    return true;
}
