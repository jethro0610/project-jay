#include <array>
#include <math.h>
#include <numbers>
#include <glm/gtx/compatibility.hpp>
#include "SpreadManager.h"
#include "../../Helpers/ChunkHelpers.h"
#include "../../Constants/GameConstants.h"
#include "SeedManager.h"
#include "../Time.h"
#include "../../Types/Transform.h"
#include "../Components/TransformComponent.h"
#include "World.h"
#include "../../Logging/Logger.h"
using namespace glm;

SpreadManager::SpreadManager(
    ResourceManager& resourceManager, 
    SeedManager& seedManager,
    World& world
) :
    resourceManager_(resourceManager),
    seedManager_(seedManager),
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
    ivec2 normChunk = GetNormalizedChunk2D(chunk);
    SpreadChunk& spreadChunk = spreadChunks_[normChunk.x][normChunk.y]; 

    if (spreadChunk.keysToIndex.contains(key))
        return false;
    assert(spreadChunk.count != MAX_SPREAD);

    const float offset = SPREAD_DIST / 2.0f;
    vec3 position = vec3(key.x * SPREAD_DIST + offset, height, key.y * SPREAD_DIST + offset);
    position = world_.GetNearestInDirection(position, -Transform::worldUp, 16);
    spreadChunk.positions[spreadChunk.count] = position; 
    spreadChunk.keys[spreadChunk.count] = key;
    spreadChunk.keysToIndex[key] = spreadChunk.count;
    spreadChunk.count++;
    dirtyChunks_.insert(chunk);

    return true;
}

bool SpreadManager::AddSpread(glm::vec3 position) {
    return AddSpread(WorldPositionToSpreadKey(position), position.y);
}

AddSpreadInfo SpreadManager::AddSpread(glm::vec3 position, int radius, uint16_t amount) {
    radius--;
    ivec2 origin = WorldPositionToSpreadKey(position);
    uint16_t count = 0;

    // Get the spreads we can add in this radius
    viableAddKeys_.Clear();
    for (int x = -radius; x <= radius; x++) {
    for (int z = -radius; z <= radius; z++) {
        float distance = sqrtf(x*x + z*z);
        if (distance > radius)
            continue;
        
        ivec2 key = origin + ivec2(x, z);
        if (!SpreadIsActive(key))
            viableAddKeys_.Append(key);
    } }

    // Randomly select a spread from the viable ones and
    // add it
    while (viableAddKeys_.GetCount() > 0 && count < amount) {
        uint32_t index = std::rand() % viableAddKeys_.GetCount();
        AddSpread(viableAddKeys_[index], position.y);
        viableAddKeys_.Remove(index);
        count++;
    }

    return AddSpreadInfo{count, origin};
}

bool SpreadManager::RemoveSpread(ivec2 key, vec3 seedOffset) {
    ivec2 chunk = SpreadKeyToChunk(key); 
    ivec2 normChunk = GetNormalizedChunk2D(chunk);
    SpreadChunk& spreadChunk = spreadChunks_[normChunk.x][normChunk.y]; 

    if (!spreadChunk.keysToIndex.contains(key))
        return false;
    uint16_t deletedIndex = spreadChunk.keysToIndex[key];
    vec3 seedPosition = spreadChunk.positions[deletedIndex] + vec3(0.0f, 0.25f, 0.0f);
    seedManager_.CreateSeed(seedPosition, seedOffset);

    spreadChunk.count--;
    vec3 lastPosition = spreadChunk.positions[spreadChunk.count];
    ivec2 lastKey = spreadChunk.keys[spreadChunk.count];

    spreadChunk.positions[deletedIndex] = lastPosition;
    spreadChunk.keys[deletedIndex] = lastKey;
    spreadChunk.keysToIndex[lastKey] = deletedIndex;
    spreadChunk.keysToIndex.erase(key);
    dirtyChunks_.insert(chunk);
    return true;
}

bool SpreadManager::RemoveSpread(vec3 position, vec3 seedOffset) {
    ivec2 key = WorldPositionToSpreadKey(position);
    return RemoveSpread(key, seedOffset);
}

uint16_t SpreadManager::RemoveSpread(vec3 position, int16_t radius, vec3 seedOffset) {
    uint32_t count = 0;
    ivec2 origin = WorldPositionToSpreadKey(position);
    for (int x = -radius; x <= radius; x++) {
    for (int z = -radius; z <= radius; z++) {
        if (sqrt(x*x + z*z) > radius)
            continue;
        if (RemoveSpread(origin + ivec2(x, z), seedOffset))
            count++;
    } }
    return count;
}
