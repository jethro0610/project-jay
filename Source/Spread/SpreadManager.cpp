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
    terrain_(terrain),
    count_(0)
{ 
    for (int i = 0; i < KEY_LENGTH; i++) {
    for (int j = 0; j < KEY_LENGTH; j++) {
        spreadKeys_[i][j] = -1;
    }}
    particleTimer_ = 0.0f;
}

void SpreadManager::Update(float deltaTime) {
    particleTimer_ += deltaTime;
    if (particleTimer_ > 0.5f) {
        particleTimer_ = 0.0f;
        int spreadIndex = rand() % renderData_.size();
        vec3 position = renderData_[spreadIndex].modelMatrix[3];
        SpreadParticle particle;
        particle.position = vec4(position, 1.0f);
        spreadParticles_.push_back(particle);
    }

    for (int i = 0; i < spreadParticles_.size(); i++) {
        SpreadParticle& particle = spreadParticles_[i];
        particle.time += deltaTime;
        if (particle.time > 5.0f) {
            spreadParticles_.remove(i);
            i--;
            continue;
        }

        particle.position.y += deltaTime;
    }
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
    return spreadKeys_[key.x][key.y] != -1;
}

bool SpreadManager::SpreadIsActive(const vec3& position) const {
    return SpreadIsActive(vec2(position.x, position.z));
}

bool SpreadManager::AddSpread(const ivec2& key) {
    if (spreadKeys_[key.x][key.y] != -1)
        return false;

    const float offset = SPREAD_DIST / 2.0f;

    Transform transform;

    transform.position = vec3(
        (key.x - KEY_LENGTH / 2) * SPREAD_DIST + offset, 
        0.0f, 
        (key.y - KEY_LENGTH / 2) * SPREAD_DIST + offset 
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

    SpreadRenderData renderData;
    renderData.modelMatrix = transform.ToMatrix();

    renderData.color.r = RandomFloatRange(0.95f, 1.0f);
    renderData.color.g = RandomFloatRange(0.85f, 0.95f);
    renderData.color.b = RandomFloatRange(0.65f, 0.75f);
    renderData.time = GlobalTime::GetTime();

    int addIndex = renderData_.push_back(renderData);
    spreadKeys_[key.x][key.y] = addIndex;
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

    for (int x = -radius; x <= radius; x++) {
    for (int z = -radius; z <= radius; z++) {
        float distance = sqrtf(x*x + z*z);
        if (distance > radius)
            continue;
        
        ivec2 key = origin + ivec2(x, z);
        if (AddSpread(key))
            count++;
    } }

    return AddSpreadInfo{count, origin};
}

bool SpreadManager::RemoveSpread(
    const SpreadKey& key, 
    bool createSeed,
    Entity* remover,
    const vec3& seedOffset
) {
    if (spreadKeys_[key.x][key.y] == -1)
        return false;

    // Get the index of the spread to delete 
    int indexToRemove = spreadKeys_[key.x][key.y];
    if (createSeed) {
        vec3 position = vec3(renderData_[indexToRemove].modelMatrix[3]);
        vec3 seedPosition = position + vec3(0.0f, 0.25f, 0.0f);
        seedManager_.CreateSeed(seedPosition, remover, seedOffset);
    }

    // Remove the element at the index...
    renderData_.remove(indexToRemove);
    spreadKeys_[key.x][key.y] = -1;

    // ...and get the key of the spread that
    // replaced that index...
    vec3 swappedPosition = renderData_[indexToRemove].modelMatrix[3];
    SpreadKey swapKey = GetKey(vec2(swappedPosition.x, swappedPosition.z));

    // ...then the index to the key
    spreadKeys_[swapKey.x][swapKey.y] = indexToRemove;
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


void SpreadManager::Reset() {
    count_ = 0;
    for (int x = 0; x < KEY_LENGTH; x++) {
    for (int y = 0; y < KEY_LENGTH; y++) {
        spreadKeys_[x][y] = -1;
    }}
    renderData_.clear();
}

float SpreadManager::GetCoverage() {
    float terrainUnitArea = (1.0f / TerrainConsts::WORLD_TO_TERRAIN_SCALAR); 
    terrainUnitArea *= terrainUnitArea;
    float spreadUnitArea = SpreadManager::SPREAD_DIST * SpreadManager::SPREAD_DIST;

    float terrainArea = terrain_.area_ * terrainUnitArea;
    int maxSpread = terrainArea / spreadUnitArea; 
    return GetCount() / (float)maxSpread;
}
