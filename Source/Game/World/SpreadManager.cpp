#include "SpreadManager.h"
#include "../../Helpers/ChunkHelpers.h"
using namespace glm;


SpreadManager::SpreadManager(ResourceManager& resourceManager, World& world):
    resourceManager_(resourceManager),
    world_(world)
{

}

ivec2 SpreadManager::WorldPositionToSpreadKey(vec3 position) const {
    vec2 spreadOrigin(position.x, position.z);
    spreadOrigin /= SPREAD_DIST;
    spreadOrigin = floor(spreadOrigin);
    return ivec2(spreadOrigin);
}

ivec2 SpreadManager::SpreadKeyToChunk(ivec2 key) const {
    ivec2 chunkPos = (key * (int)SPREAD_DIST) / (int)CHUNK_SIZE;
    chunkPos = GetNormalizedChunk2D(chunkPos);
    return chunkPos;
}

bool SpreadManager::SpreadIsActive(ivec2 key) const {
    ivec2 chunkPos = SpreadKeyToChunk(key); 
    const SpreadChunk& chunk = chunks_[chunkPos.x][chunkPos.y]; 
    return chunk.keysToIndex.contains(key);
}

bool SpreadManager::AddSpread(ivec2 key, float height) {
    ivec2 chunkPos = SpreadKeyToChunk(key); 
    SpreadChunk& chunk = chunks_[chunkPos.x][chunkPos.y]; 

    if (chunk.keysToIndex.contains(key))
        return false;
    assert(chunk.count != MAX_SPREAD);

    const float offset = SPREAD_DIST / 2.0f;
    vec3 position = vec3(key.x * SPREAD_DIST + offset, height, key.y * SPREAD_DIST + offset);
    position = world_.GetNearestInDirection(position, -Transform::worldUp);
    chunk.positions[chunk.count] = position; 
    chunk.keys[chunk.count] = key;
    chunk.keysToIndex[key] = chunk.count;
    chunk.count++;

    dirtyChunks_.insert(chunkPos);

    return true;
}

AddSpreadInfo SpreadManager::AddSpread(glm::vec3 position) {
    AddSpreadInfo returnInfo;
    returnInfo.key = WorldPositionToSpreadKey(position);
    returnInfo.added = AddSpread(returnInfo.key , position.y);
    return returnInfo;
}


AddSpreadInfo SpreadManager::AddSpread(glm::vec3 position, int radius) {
    ivec2 origin = WorldPositionToSpreadKey(position);
    bool added = false;
    for (int x = -radius; x <= radius; x++)
    for (int z = -radius; z <= radius; z++) {
        if (sqrt(x*x + z*z) > radius)
            continue;
        if (AddSpread(origin + ivec2(x, z), position.y))
            added = true;
    }
    return AddSpreadInfo{added, origin};
}

bool SpreadManager::RemoveSpread(ivec2 key) {
    ivec2 chunkPos = SpreadKeyToChunk(key); 
    SpreadChunk& chunk = chunks_[chunkPos.x][chunkPos.y]; 

    if (!chunk.keysToIndex.contains(key))
        return false;

    chunk.count--;
    uint16_t deletedIndex = chunk.keysToIndex[key];
    vec3 lastPosition = chunk.positions[chunk.count];
    ivec2 lastKey = chunk.keys[chunk.count];

    chunk.positions[deletedIndex] = lastPosition;
    chunk.keys[deletedIndex] = lastKey;
    chunk.keysToIndex[lastKey] = deletedIndex;
    chunk.keysToIndex.erase(key);
    dirtyChunks_.insert(chunkPos);
    return true;
}

bool SpreadManager::RemoveSpread(vec3 position) {
    ivec2 key = WorldPositionToSpreadKey(position);
    return RemoveSpread(key);
}

void SpreadManager::RemoveSpread(vec3 position, int radius) {
    ivec2 origin = WorldPositionToSpreadKey(position);
    for (int x = -radius; x <= radius; x++)
    for (int z = -radius; z <= radius; z++) {
        if (sqrt(x*x + z*z) > radius)
            continue;
        RemoveSpread(origin + ivec2(x, z)); 
    }
}
