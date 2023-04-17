#include "SpreadManager.h"
#include "../../Helpers/ChunkHelpers.h"
#include "../../Logging/Logger.h"
#include <gtx/string_cast.hpp>
using namespace glm;


SpreadManager::SpreadManager(std::unordered_set<glm::ivec2>& dirtyChunks2D) :
    dirtyChunks2D_(dirtyChunks2D)
{ 

}

ivec2 SpreadManager::WorldPositionToSpreadKey(vec3 position) const {
    vec2 spreadOrigin(position.x, position.z);
    spreadOrigin /= SPREAD_DIST;
    spreadOrigin = floor(spreadOrigin);
    return ivec2(spreadOrigin);
}

ivec2 SpreadManager::SpreadKeyToChunk(ivec2 key) const {
    vec2 keyPos = vec2(key) * SPREAD_DIST;
    return GetChunkAtWorldPosition2D(keyPos);
}

bool SpreadManager::SpreadIsActive(ivec2 key) const {
    ivec2 chunk = SpreadKeyToChunk(key); 
    chunk = GetNormalizedChunk2D(chunk);
    const SpreadChunk& spreadChunk = spreadChunks_[chunk.x][chunk.y]; 
    return spreadChunk.keysToIndex.contains(key);
}

bool SpreadManager::AddSpread(ivec2 key, float height) {
    ivec2 chunk = SpreadKeyToChunk(key); 
    dirtyChunks2D_.insert(chunk);
    chunk = GetNormalizedChunk2D(chunk);
    SpreadChunk& spreadChunk = spreadChunks_[chunk.x][chunk.y]; 

    if (spreadChunk.keysToIndex.contains(key))
        return false;
    assert(spreadChunk.count != MAX_SPREAD);

    const float offset = SPREAD_DIST / 2.0f;
    vec2 position = vec2(key.x * SPREAD_DIST + offset,key.y * SPREAD_DIST + offset);
    spreadChunk.positions[spreadChunk.count] = position; 
    spreadChunk.keys[spreadChunk.count] = key;
    spreadChunk.keysToIndex[key] = spreadChunk.count;
    spreadChunk.count++;

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
    ivec2 chunk = SpreadKeyToChunk(key); 
    dirtyChunks2D_.insert(chunk);
    chunk = GetNormalizedChunk2D(chunk);
    SpreadChunk& spreadChunk = spreadChunks_[chunk.x][chunk.y]; 

    if (!spreadChunk.keysToIndex.contains(key))
        return false;

    spreadChunk.count--;
    uint16_t deletedIndex = spreadChunk.keysToIndex[key];
    vec2 lastPosition = spreadChunk.positions[spreadChunk.count];
    ivec2 lastKey = spreadChunk.keys[spreadChunk.count];

    spreadChunk.positions[deletedIndex] = lastPosition;
    spreadChunk.keys[deletedIndex] = lastKey;
    spreadChunk.keysToIndex[lastKey] = deletedIndex;
    spreadChunk.keysToIndex.erase(key);
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
