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
#include "Logging/Logger.h"
using namespace glm;

SpreadManager::SpreadManager(
    SeedManager& seedManager,
    Terrain& terrain 
) :
    seedManager_(seedManager),
    terrain_(terrain)
{ 
    for (int i = 0; i < KEY_LENGTH; i++) {
    for (int j = 0; j < KEY_LENGTH; j++) {
        keys_[i][j] = {-1, false};
    }}
}

SpreadKey SpreadManager::GetKey(const vec2& position) const {
    SpreadKey key = ivec2(floor(position / SPREAD_DIST)) + ivec2(KEY_LENGTH / 2);
    return key;
}

SpreadKey SpreadManager::GetKey(const vec3& position) const {
    return GetKey(vec2(position.x, position.z));
}

bool SpreadManager::SpreadIsActive(const vec2& position) const {
    SpreadKey key = GetKey(position);
    return keys_[key.x][key.y].index != -1;
}

bool SpreadManager::SpreadIsActive(const vec3& position) const {
    return SpreadIsActive(vec2(position.x, position.z));
}

bool SpreadManager::AddSpread(const ivec2& key, bool weed) {
    if (keys_[key.x][key.y].index != -1)
        return false;

    const float offset = SPREAD_DIST / 2.0f;

    Transform transform;

    transform.position = vec3(
        (key.x - KEY_LENGTH * 0.5f) * SPREAD_DIST + offset, 
        0.0f, 
        (key.y - KEY_LENGTH * 0.5f) * SPREAD_DIST + offset 
    );
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

    RenderData renderData;
    renderData.modelMatrix = transform.ToMatrix();
    if (any(isnan(renderData.modelMatrix[3]))) {
        return false;
    }

    renderData.color.r = RandomFloatRange(0.95f, 1.0f);
    renderData.color.g = RandomFloatRange(0.85f, 0.95f);
    renderData.color.b = RandomFloatRange(0.65f, 0.75f);
    renderData.time = GlobalTime::GetTime();

    int addIndex = weed ? weedData_.push_back(renderData) : spreadData_.push_back(renderData);
    keys_[key.x][key.y].index = addIndex;
    keys_[key.x][key.y].weed = weed;

    return true;
}

bool SpreadManager::AddSpread(const glm::vec3& position, bool weed) {
    return AddSpread(GetKey(position), weed);
}

AddSpreadInfo SpreadManager::AddSpread(const glm::vec3& position, int radius, int amount, bool weed) {
    radius--;
    ivec2 origin = GetKey(position);
    int count = 0;

    for (int x = -radius; x <= radius; x++) {
    for (int z = -radius; z <= radius; z++) {
        float distance = sqrtf(x*x + z*z);
        if (distance > radius)
            continue;
        
        ivec2 key = origin + ivec2(x, z);
        if (AddSpread(key, weed))
            count++;
    } }

    return AddSpreadInfo{count, origin};
}

bool SpreadManager::RemoveSpread(
    const SpreadKey& key, 
    Entity* remover
) {
    if (key.x < 0 || key.y < 0 || key.x >= KEY_LENGTH || key.y >= KEY_LENGTH)
        return false;

    if (keys_[key.x][key.y].index == -1)
        return false;

    vector_contig<RenderData, MAX_SPREAD>& data = keys_[key.x][key.y].weed ? weedData_ : spreadData_;

    // Get the index of the spread to delete 
    int indexToRemove = keys_[key.x][key.y].index;

    // Remove the element at the index...
    data.remove(indexToRemove);
    keys_[key.x][key.y].index = -1;

    // Skip swapping if the last element was removed
    if (indexToRemove >= data.size())
        return true;

    // ...and get the key of the spread that
    // replaced that index...
    vec3 swappedPosition = data[indexToRemove].modelMatrix[3];
    SpreadKey swapKey = GetKey(vec2(swappedPosition.x, swappedPosition.z));

    // ...then the index to the key
    keys_[swapKey.x][swapKey.y].index = indexToRemove;

    vec2 position = vec2(key - ivec2(KEY_LENGTH / 2)) * SPREAD_DIST;
    vec2 offset = RandomVector2D(SPREAD_DIST / 2.0f);
    position += offset;
    seedManager_.CreateSeed(vec3(position.x, 0.0f, position.y), nullptr, vec3(0.0f, 1.0f, 0.0f));

    return true;
}

bool SpreadManager::RemoveSpread(
    const vec3& position, 
    Entity* remover
) {
    SpreadKey key = GetKey(position);
    return RemoveSpread(key, remover);
}

int SpreadManager::RemoveSpread(
    const vec3& position, 
    int radius, 
    Entity* remover
) {
    int count = 0;
    ivec2 origin = GetKey(position);
    for (int x = -radius; x <= radius; x++) {
    for (int z = -radius; z <= radius; z++) {
        if (sqrt(x*x + z*z) > radius)
            continue;
        if (RemoveSpread(origin + ivec2(x, z), remover))
            count++;
    } }
    return count;
}

int SpreadManager::RemoveSpread(
    const vec3& position, 
    float radius, 
    Entity* remover
) {
    int intRadius = (radius) / SpreadManager::SPREAD_DIST;
    return RemoveSpread(position, intRadius, remover);
}


void SpreadManager::Reset() {
    for (int x = 0; x < KEY_LENGTH; x++) {
    for (int y = 0; y < KEY_LENGTH; y++) {
        keys_[x][y] = {-1, false};
    }}
    spreadData_.clear();
    weedData_.clear();
}

float SpreadManager::GetCoverage() {
    float terrainUnitArea = (1.0f / TerrainConsts::WORLD_TO_TERRAIN_SCALAR); 
    terrainUnitArea *= terrainUnitArea;
    float spreadUnitArea = SpreadManager::SPREAD_DIST * SpreadManager::SPREAD_DIST;

    float terrainArea = terrain_.area_ * terrainUnitArea;
    int maxSpread = terrainArea / spreadUnitArea; 
    return (GetCount() - GetWeedCount()) / (float)maxSpread;
}

void SpreadManager::GetWeedLocations(std::vector<ivec2>& locations) {
    for (int x = 0; x < KEY_LENGTH; x++) {
    for (int y = 0; y < KEY_LENGTH; y++) {
        if (keys_[x][y].index != -1 && keys_[x][y].weed)
            locations.push_back(ivec2(x,y)); 
    }}
}
