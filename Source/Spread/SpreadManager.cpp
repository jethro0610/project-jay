#include "SpreadManager.h"
#include "Seed/SeedManager.h"
#include "Time/Time.h"
#include "Terrain/Terrain.h"
#include "Types/Transform.h"
#include "Helpers/Random.h"
#include <math.h>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>
using namespace glm;

SpreadManager::SpreadManager(
    SeedManager& seedManager,
    Terrain& terrain 
) :
    seedManager_(seedManager),
    terrain_(terrain),
    count_(0)
{ 

}

SpreadKey SpreadManager::GetKey(const vec2& position) const {
    return ivec2(floor(position / SPREAD_DIST));
}

SpreadKey SpreadManager::GetKey(const vec3& position) const {
    return GetKey(vec2(position.x, position.z));
}

bool SpreadManager::SpreadIsActive(const vec2& position) const {
    SpreadKey key = GetKey(position);
    return keyIndices_.contains(key);
}

bool SpreadManager::SpreadIsActive(const vec3& position) const {
    return SpreadIsActive(vec2(position.x, position.z));
}

bool SpreadManager::AddSpread(const ivec2& key) {
    if (keyIndices_.contains(key) || tramples_.contains(key))
        return false;

    const float offset = SPREAD_DIST / 2.0f;

    Transform transform;

    transform.position = vec3(key.x * SPREAD_DIST + offset, 0.0f, key.y * SPREAD_DIST + offset);
    vec2 randOffset = RandomVector2D(1.0f);
    transform.position.x += randOffset.x;
    transform.position.z += randOffset.y;
    vec2 pos2d = vec2(transform.position.x, transform.position.z);
    transform.position.y = terrain_.GetHeight(pos2d) + RandomFloatRange(0.15f, 1.00f);
    transform.scale = vec3(RandomFloatRange(0.35f, 1.0f));
    transform.rotation = angleAxis(RandomFloatRange(0, 360.0f), Transform::worldUp);
    transform.rotation = quat(orientation(terrain_.GetNormal(pos2d), Transform::worldUp)) * transform.rotation;
    vec3 randomEuler = RandomVector(radians(15.0f));
    transform.rotation = quat(randomEuler) * transform.rotation;

    SpreadRenderData renderData;
    renderData.modelMatrix = transform.ToMatrix();

    renderData.color.r = RandomFloatRange(0.95f, 1.0f);
    renderData.color.g = RandomFloatRange(0.85f, 0.95f);
    renderData.color.b = RandomFloatRange(0.65f, 0.75f);
    renderData.time = GlobalTime::GetTime();

    keyIndices_[key] = renderData_.push_back(renderData);
    count_++;

    return true;
}

bool SpreadManager::AddSpread(const glm::vec3& position) {
    return AddSpread(GetKey(position));
}

AddSpreadInfo SpreadManager::AddSpread(const glm::vec3& position, int radius, int amount) {
    radius--;
    ivec2 origin = GetKey(position);
    int count = 0;

    // Get the spreads we can add in this radius
    viableAddKeys_.clear();
    for (int x = -radius; x <= radius; x++) {
    for (int z = -radius; z <= radius; z++) {
        float distance = sqrtf(x*x + z*z);
        if (distance > radius)
            continue;
        
        ivec2 key = origin + ivec2(x, z);
        if (!keyIndices_.contains(key))
            viableAddKeys_.push_back(key);
    } }

    // Randomly select a spread from the viable ones and
    // add it
    while (viableAddKeys_.size() > 0 && count < amount) {
        int index = std::rand() % viableAddKeys_.size();
        AddSpread(viableAddKeys_[index]);
        viableAddKeys_.remove(index);
        count++;
    }

    return AddSpreadInfo{count, origin};
}

bool SpreadManager::RemoveSpread(
    const SpreadKey& key, 
    bool createSeed,
    Entity* remover,
    const vec3& seedOffset
) {
    auto foundKey = keyIndices_.find(key);
    if (foundKey == keyIndices_.end())
        return false;

    int deleteIndex = foundKey->second;
    vec3 position = vec3(renderData_[deleteIndex].modelMatrix[3]);

    if (createSeed) {
        vec3 seedPosition = position + vec3(0.0f, 0.25f, 0.0f);
        seedManager_.CreateSeed(seedPosition, remover, seedOffset);
    }

    mat4 swappedTransform = renderData_.remove(deleteIndex).modelMatrix;
    vec3 swappedPosition = vec3(swappedTransform[3]);
    SpreadKey keyToSwap = GetKey(vec2(swappedPosition.x, swappedPosition.z));

    keyIndices_[keyToSwap] = deleteIndex;
    keyIndices_.erase(key);
    count_--;

    return true;
}

bool SpreadManager::RemoveSpread(
    const vec3& position, 
    bool createSeed,
    Entity* remover,
    const vec3& seedOffset
) {
    SpreadKey key = GetKey(position);
    return RemoveSpread(key, createSeed, remover, seedOffset);
}

int SpreadManager::RemoveSpread(
    const vec3& position, 
    int radius, 
    bool createSeed,
    Entity* remover,
    const vec3& seedOffset
) {
    int count = 0;
    ivec2 origin = GetKey(position);
    for (int x = -radius; x <= radius; x++) {
    for (int z = -radius; z <= radius; z++) {
        if (sqrt(x*x + z*z) > radius)
            continue;
        if (RemoveSpread(origin + ivec2(x, z), createSeed, remover, seedOffset))
            count++;
    } }
    return count;
}

void SpreadManager::Trample(const SpreadKey& key) {
    RemoveSpread(key);
    tramples_.insert(key); 
}

void SpreadManager::Trample(const glm::vec3& position) {
    Trample(GetKey(position));
}

void SpreadManager::Trample(const glm::vec3& position, int radius) {
    if (radius == 0)
        return;

    ivec2 origin = GetKey(position);
    radius--;
    
    for (int x = -radius; x <= radius; x++) {
    for (int z = -radius; z <= radius; z++) {
        float distance = sqrtf(x*x + z*z);
        if (distance > radius)
            continue;
        
        ivec2 key = origin + ivec2(x, z);
        Trample(key);
    } }
}

void SpreadManager::Reset() {
    count_ = 0;
    renderData_.clear();
    viableAddKeys_.clear();
    keyIndices_.clear();
    tramples_.clear();
}
