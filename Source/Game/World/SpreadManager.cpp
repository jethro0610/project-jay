#include <array>
#include <math.h>
#include <numbers>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/string_cast.hpp>
#include "SpreadManager.h"
#include "../../Helpers/Assert.h"
#include "../../Constants/GameConstants.h"
#include "SeedManager.h"
#include "../Time.h"
#include "../../Types/Transform.h"
#include "../Components/TransformComponent.h"
#include "World.h"
#include "../../Logging/Logger.h"
using namespace glm;

SpreadManager::SpreadManager(
    SeedManager& seedManager,
    World& world
) :
    seedManager_(seedManager),
    world_(world),
    count_(0)
{ 

}

SpreadKey SpreadManager::GetKey(vec2 position) const {
    position /= SPREAD_DIST;
    position = floor(position);
    return ivec2(position);
}

SpreadKey SpreadManager::GetKey(vec3 position) const {
    return GetKey(vec2(position.x, position.z));
}

bool SpreadManager::SpreadIsActive(vec2 position) const {
    SpreadKey key = GetKey(position);
    return keyIndices_.contains(key);
}

bool SpreadManager::SpreadIsActive(vec3 position) const {
    return SpreadIsActive(vec2(position.x, position.z));
}

bool SpreadManager::AddSpread(ivec2 key) {
    if (keyIndices_.contains(key))
        return false;
    ASSERT((positions_.GetCount() <= MAX_SPREAD), "Spread count exceeds max");

    const float offset = SPREAD_DIST / 2.0f;
    vec2 position2d = vec2(key.x * SPREAD_DIST + offset, key.y * SPREAD_DIST + offset);
    float worldHeight = world_.GetHeight(position2d);

    vec4 position = vec4(position2d.x, worldHeight, position2d.y, 0.0f);
    keyIndices_[key] = positions_.Append(position);
    count_++;

    return true;
}

bool SpreadManager::AddSpread(glm::vec3 position) {
    return AddSpread(GetKey(position));
}

AddSpreadInfo SpreadManager::AddSpread(glm::vec3 position, int radius, int amount) {
    radius--;
    ivec2 origin = GetKey(position);
    int count = 0;

    // Get the spreads we can add in this radius
    viableAddKeys_.Clear();
    for (int x = -radius; x <= radius; x++) {
    for (int z = -radius; z <= radius; z++) {
        float distance = sqrtf(x*x + z*z);
        if (distance > radius)
            continue;
        
        ivec2 key = origin + ivec2(x, z);
        if (!keyIndices_.contains(key))
            viableAddKeys_.Append(key);
    } }

    // Randomly select a spread from the viable ones and
    // add it
    while (viableAddKeys_.GetCount() > 0 && count < amount) {
        int index = std::rand() % viableAddKeys_.GetCount();
        AddSpread(viableAddKeys_[index]);
        viableAddKeys_.Remove(index);
        count++;
    }

    return AddSpreadInfo{count, origin};
}

bool SpreadManager::RemoveSpread(
    SpreadKey key, 
    EntityID remover,
    vec3 seedOffset
) {
    auto foundKey = keyIndices_.find(key);
    if (foundKey == keyIndices_.end())
        return false;

    int deleteIndex = foundKey->second;

    vec3 seedPosition = vec3(positions_[deleteIndex]) + vec3(0.0f, 0.25f, 0.0f);
    seedManager_.CreateSeed(seedPosition, remover, seedOffset);

    vec4 swappedPosition = positions_.Remove(deleteIndex);
    SpreadKey keyToSwap = GetKey(vec2(swappedPosition.x , swappedPosition.z));

    keyIndices_[keyToSwap] = deleteIndex;
    keyIndices_.erase(key);
    count_--;

    return true;
}

bool SpreadManager::RemoveSpread(
    vec3 position, 
    EntityID remover,
    vec3 seedOffset
) {
    SpreadKey key = GetKey(position);
    return RemoveSpread(key, remover, seedOffset);
}

int SpreadManager::RemoveSpread(
    vec3 position, 
    int radius, 
    EntityID remover,
    vec3 seedOffset
) {
    int count = 0;
    ivec2 origin = GetKey(position);
    for (int x = -radius; x <= radius; x++) {
    for (int z = -radius; z <= radius; z++) {
        if (sqrt(x*x + z*z) > radius)
            continue;
        if (RemoveSpread(origin + ivec2(x, z), remover, seedOffset))
            count++;
    } }
    return count;
}
