#include "SpreadManager.h"
#include "Seed/SeedManager.h"
#include "Time/Time.h"
#include "Terrain/Terrain.h"
#include "Types/Transform.h"
#include "Helpers/Random.h"
#include "Entity/Entity.h"
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
        keys_[i][j] = {-1, SpreadType_Flower};
    }}
}

SpreadKey SpreadManager::GetKey(const vec2& position) const {
    SpreadKey key = ivec2(floor(position / SPREAD_DIST)) + ivec2(KEY_LENGTH / 2);
    return key;
}

SpreadKey SpreadManager::GetKey(const vec3& position) const {
    return GetKey(vec2(position.x, position.z));
}

vec2 SpreadManager::KeyToPosition(const SpreadKey& key) const {
    vec2 position = vec2(key - ivec2(KEY_LENGTH / 2)) * SPREAD_DIST;
    vec2 offset = RandomVector2D(SPREAD_DIST / 2.0f);
    position += offset;
    return position + offset;
}

bool SpreadManager::SpreadIsActive(const vec2& position) const {
    SpreadKey key = GetKey(position);
    return keys_[key.x][key.y].index != -1;
}

bool SpreadManager::SpreadIsActive(const vec3& position) const {
    return SpreadIsActive(vec2(position.x, position.z));
}

bool SpreadManager::AddSpread(const ivec2& key, SpreadType type) {
    if (keys_[key.x][key.y].index != -1)
        return false;
    if (tramples_[key.x][key.y] == true)
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

    //renderData.color.r = RandomFloatRange(0.95f, 1.0f);
    //renderData.color.g = RandomFloatRange(0.85f, 0.95f);
    //renderData.color.b = RandomFloatRange(0.65f, 0.75f);
    renderData.time = GlobalTime::GetTime();
    renderData.active = RenderData::ACTIVE_SPREAD;

    int addIndex = spreadData_[type].push_back(renderData);
    keys_[key.x][key.y].index = addIndex;
    keys_[key.x][key.y].type = type;

    return true;
}

bool SpreadManager::AddSpread(const glm::vec3& position, SpreadType type) {
    return AddSpread(GetKey(position), type);
}

AddSpreadInfo SpreadManager::AddSpread(
    const glm::vec3& position, 
    int radius, 
    float density, 
    AddSpreadDistribution distribution,
    SpreadType type
) {
    radius--;
    ivec2 origin = GetKey(position);
    int count = 0;

    for (int x = -radius; x <= radius; x++) {
    for (int z = -radius; z <= radius; z++) {
        float distance = sqrtf(x*x + z*z);
        if (distance > radius)
            continue;

        float t = 1.0f;
        if (distribution == AD_Feather)
            t = 1.0f - (distance / radius);

        float rand = RandomFloatRange(0.0, 1.0f);
        if (rand > density * t)
            continue;
        
        ivec2 key = origin + ivec2(x, z);
        if (AddSpread(key, type))
            count++;
    } }

    return AddSpreadInfo{count, origin};
}

AddSpreadInfo SpreadManager::AddSpread(
    const glm::vec3& position, 
    float radius, 
    float density, 
    AddSpreadDistribution distribution,
    SpreadType type
) {
    int intRadius = (radius) / SpreadManager::SPREAD_DIST;
    return AddSpread(position, intRadius, density, distribution, type);
}


bool SpreadManager::RemoveSpread(
    const SpreadKey& key, 
    Entity* remover,
    bool deactivate
) {
    if (key.x < 0 || key.y < 0 || key.x >= KEY_LENGTH || key.y >= KEY_LENGTH)
        return false;

    if (keys_[key.x][key.y].index == -1)
        return false;

    // Get the index of the spread to delete 
    int indexToRemove = keys_[key.x][key.y].index;
    SpreadType type = keys_[key.x][key.y].type;

    if (spreadData_[type][indexToRemove].active == RenderData::INACTIVE_SPREAD)
        return false;

    if (deactivate) {
        spreadData_[type][indexToRemove].active = RenderData::INACTIVE_SPREAD;
    }
    else {
        // Remove the element at the index...
        spreadData_[type].remove(indexToRemove);
        keys_[key.x][key.y].index = -1;

        // Skip swapping if the last element was removed
        if (indexToRemove >= spreadData_[type].size())
            return true;

        if (keys_[key.x][key.y].index != -1)
            return false;
        // ...and get the key of the spread that
        // replaced that index...
        vec3 swappedPosition = spreadData_[type][indexToRemove].modelMatrix[3];
        SpreadKey swapKey = GetKey(vec2(swappedPosition.x, swappedPosition.z));

        // ...then the index to the key
        keys_[swapKey.x][swapKey.y].index = indexToRemove;
    }

    vec2 position = KeyToPosition(key);
    seedManager_.CreateSeed(vec3(position.x, terrain_.GetHeight(position, TA_Low), position.y), remover, vec3(0.0f, 8.0f, 0.0f));

    return true;
}

bool SpreadManager::RemoveSpread(
    const vec3& position, 
    Entity* remover,
    bool deactivate
) {
    SpreadKey key = GetKey(position);
    return RemoveSpread(key, remover, deactivate);
}

int SpreadManager::RemoveSpread(
    const vec3& position, 
    int radius, 
    Entity* remover,
    bool deactivate
) {
    int count = 0;
    ivec2 origin = GetKey(position);
    for (int x = -radius; x <= radius; x++) {
    for (int z = -radius; z <= radius; z++) {
        if (sqrt(x*x + z*z) > radius)
            continue;
        if (RemoveSpread(origin + ivec2(x, z), remover, deactivate))
            count++;
    } }
    return count;
}

int SpreadManager::RemoveSpread(
    const vec3& position, 
    float radius, 
    Entity* remover,
    bool deactivate
) {
    int intRadius = (radius) / SpreadManager::SPREAD_DIST;
    return RemoveSpread(position, intRadius, remover);
}

int SpreadManager::RemoveSpread(
    Cone& cone, 
    Entity* remover,
    bool deactivate
) {
    int radius = (cone.distance) / SpreadManager::SPREAD_DIST;
    int count = 0;
    ivec2 origin = GetKey(cone.position);
    vec2 normConeDir = normalize(vec2(cone.direction.x, cone.direction.z));
    for (int x = -radius; x <= radius; x++) {
    for (int z = -radius; z <= radius; z++) {
        if (sqrt(x*x + z*z) > radius)
            continue;
        ivec2 offset = ivec2(x, z);
        vec2 normOffset = normalize(vec2(offset));
        if (dot(normConeDir, normOffset) < cone.angle)
            continue;
        if (RemoveSpread(origin + offset, remover, deactivate))
            count++;
    }}
    return count;
}

void SpreadManager::ClearTramples() {
    memset(tramples_, 0, sizeof(bool) * KEY_LENGTH * KEY_LENGTH);
}

bool SpreadManager::Trample(const SpreadKey& key, Entity* trampler) {
    tramples_[key.x][key.y] = true;
    return RemoveSpread(key, trampler);
}

bool SpreadManager::Trample(const vec3& position, Entity* trampler) {
    return Trample(GetKey(position));
}

int SpreadManager::Trample(const vec3& position, float radius, Entity* trampler) {
    int intRadius = (radius) / SpreadManager::SPREAD_DIST;

    int count = 0;
    ivec2 origin = GetKey(position);
    for (int x = -intRadius; x <= intRadius; x++) {
    for (int z = -intRadius; z <= intRadius; z++) {
        if (sqrt(x*x + z*z) > intRadius)
            continue;
        if (Trample(origin + ivec2(x, z), trampler))
            count++;
    } }
    DEBUGLOG(intRadius);
    return count;
}


void SpreadManager::Reset() {
    for (int x = 0; x < KEY_LENGTH; x++) {
    for (int y = 0; y < KEY_LENGTH; y++) {
        keys_[x][y] = {-1, SpreadType_Flower};
    }}
    for (int i = 0; i < SpreadType_Num; i++) {
        spreadData_[i].clear();
    }
}
