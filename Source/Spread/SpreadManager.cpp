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
    for (int c = 0; c < NUM_CHUNKS; c++) {
        Chunk& chunk = chunks_[c];
        chunk.active = false;

        for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            chunk.indexes[x][y] = -1; 
        }}
    }
}

ivec2 SpreadManager::WorldSpaceToSpreadSpace(const glm::vec3& position) {
    return ivec2(floor(position.x / SPREAD_DIST), floor(position.z / SPREAD_DIST));
}

SpreadManager::ChunkSpacePosition SpreadManager::SpreadSpaceToChunkSpace(const glm::ivec2& spreadSpacePos) {
    glm::ivec2 chunkPos = ivec2(
        floor(spreadSpacePos.x / (float)CHUNK_SIZE), 
        floor(spreadSpacePos.y / (float)CHUNK_SIZE)
    );
    glm::ivec2 spreadPos = spreadSpacePos - (chunkPos * CHUNK_SIZE);
    return { chunkPos, spreadPos };
}

glm::ivec2 SpreadManager::ChunkSpaceToSpreadSpace(const SpreadManager::ChunkSpacePosition& chunkSpacePos) {
    return chunkSpacePos.chunk * CHUNK_SIZE + chunkSpacePos.spread;
}

glm::vec3 SpreadManager::SpreadSpaceToWorldSpace(const glm::ivec2& position) {
    return glm::vec3(position.x * SPREAD_DIST, 0.0f, position.y * SPREAD_DIST);
}

bool SpreadManager::AddSpread(const ivec2& spreadSpacePos, SpreadType type) {
    ChunkSpacePosition chunkSpacePos = SpreadSpaceToChunkSpace(spreadSpacePos);
    if (!chunkIndexes_.contains(chunkSpacePos.chunk)) {
        for (int i = 0; i < NUM_CHUNKS; i++) {
            if (chunks_[i].active)
                continue;

            chunkIndexes_[chunkSpacePos.chunk] = i; 
            chunks_[i].active = true;
            chunks_[i].origin = chunkSpacePos.chunk;
            DEBUGLOG("activated chunk " << i << " at " << to_string(chunkSpacePos.chunk));
            break;
        }
    }
    int chunkIndex = chunkIndexes_[chunkSpacePos.chunk];
    Chunk& chunk = chunks_[chunkIndex];

    if (chunk.indexes[chunkSpacePos.spread.x][chunkSpacePos.spread.y] != -1)
        return false;
    // if (tramples_[key.x][key.y] == true)
    //     return false;

    const float offset = SPREAD_DIST / 2.0f;
    Transform transform;
    transform.position = SpreadSpaceToWorldSpace(spreadSpacePos);
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

    int addIndex = chunk.renderData[type].push_back(renderData);
    chunk.indexes[chunkSpacePos.spread.x][chunkSpacePos.spread.y] = addIndex;
    chunk.types[chunkSpacePos.spread.x][chunkSpacePos.spread.y] = type;
    chunk.count++;

    return true;
}

bool SpreadManager::AddSpread(const glm::vec3& worldSpacePos, SpreadType type) {
    return AddSpread(WorldSpaceToSpreadSpace(worldSpacePos), type);
}

AddSpreadInfo SpreadManager::AddSpread(
    const glm::vec3& worldSpacePos, 
    int radius, 
    float density, 
    AddSpreadDistribution distribution,
    SpreadType type
) {
    radius--;
    ivec2 spreadSpaceOrigin = WorldSpaceToSpreadSpace(worldSpacePos);
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
        
        ivec2 position = spreadSpaceOrigin + ivec2(x, z);
        if (AddSpread(position, type))
            count++;
    } }
    return AddSpreadInfo{count, spreadSpaceOrigin};
}

AddSpreadInfo SpreadManager::AddSpread(
    const glm::vec3& worldSpacePos, 
    float radius, 
    float density, 
    AddSpreadDistribution distribution,
    SpreadType type
) {
    int intRadius = (radius) / SpreadManager::SPREAD_DIST;
    return AddSpread(worldSpacePos, intRadius, density, distribution, type);
}


bool SpreadManager::RemoveSpread(
    const ivec2& spreadSpacePos, 
    Entity* remover,
    bool deactivate
) {
    ChunkSpacePosition chunkSpacePos = SpreadSpaceToChunkSpace(spreadSpacePos);
    if (!chunkIndexes_.contains(chunkSpacePos.chunk))
        return false;

    int chunkIndex = chunkIndexes_[chunkSpacePos.chunk];
    Chunk& chunk = chunks_[chunkIndex];
    if (chunk.indexes[chunkSpacePos.spread.x][chunkSpacePos.spread.y] == -1)
        return false;

    // Get the index of the spread to delete 
    int indexToRemove = chunk.indexes[chunkSpacePos.spread.x][chunkSpacePos.spread.y];
    SpreadType type = chunk.types[chunkSpacePos.spread.x][chunkSpacePos.spread.y];

    if (chunk.renderData[type][indexToRemove].active == RenderData::INACTIVE_SPREAD)
        return false;

    if (deactivate) {
        chunk.renderData[type][indexToRemove].active = RenderData::INACTIVE_SPREAD;
    }
    else {
        // Remove the element at the index...
        chunk.renderData[type].remove(indexToRemove);
        chunk.indexes[chunkSpacePos.spread.x][chunkSpacePos.spread.y] = -1;

        // Skip swapping if the last element was removed
        if (indexToRemove >= chunk.renderData[type].size())
            return true;

        // ...and get the position of the spread that
        // replaced that index...
        vec3 swappedWorldSpacePos = chunk.renderData[type][indexToRemove].modelMatrix[3];
        ivec2 swappedSpreadSpacePos = WorldSpaceToSpreadSpace(swappedWorldSpacePos);
        ChunkSpacePosition swappedChunkSpacePos = SpreadSpaceToChunkSpace(swappedSpreadSpacePos);

        // ...then the index to the key
        chunk.indexes[swappedChunkSpacePos.spread.x][swappedChunkSpacePos.spread.y] = indexToRemove;
    }

    vec3 seedPosition = SpreadSpaceToWorldSpace(spreadSpacePos);
    seedPosition.y = terrain_.GetHeight(seedPosition, TA_Low);
    seedManager_.CreateSeed(seedPosition, remover, vec3(0.0f, 8.0f, 0.0f));
    chunk.count--;

    if (chunk.count <= 0)
        chunk.active = false;

    return true;
}

bool SpreadManager::RemoveSpread(
    const vec3& worldSpacePos, 
    Entity* remover,
    bool deactivate
) {
    return RemoveSpread(WorldSpaceToSpreadSpace(worldSpacePos), remover, deactivate);
}

int SpreadManager::RemoveSpread(
    const vec3& worldSpacePos, 
    int radius, 
    Entity* remover,
    bool deactivate
) {
    int count = 0;
    ivec2 spreadSpaceOrigin = WorldSpaceToSpreadSpace(worldSpacePos);
    for (int x = -radius; x <= radius; x++) {
    for (int z = -radius; z <= radius; z++) {
        if (sqrt(x*x + z*z) > radius)
            continue;
        if (RemoveSpread(spreadSpaceOrigin + ivec2(x, z), remover, deactivate))
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
    ivec2 spreadSpaceOrigin = WorldSpaceToSpreadSpace(cone.position);
    vec2 normConeDir = normalize(vec2(cone.direction.x, cone.direction.z));
    for (int x = -radius; x <= radius; x++) {
    for (int z = -radius; z <= radius; z++) {
        if (sqrt(x*x + z*z) > radius)
            continue;
        ivec2 offset = ivec2(x, z);
        vec2 normOffset = normalize(vec2(offset));
        if (dot(normConeDir, normOffset) < cone.angle)
            continue;
        if (RemoveSpread(spreadSpaceOrigin + offset, remover, deactivate))
            count++;
    }}
    return count;
}

void SpreadManager::ClearTramples() {
    //memset(tramples_, 0, sizeof(bool) * KEY_LENGTH * KEY_LENGTH);
}

bool SpreadManager::Trample(const glm::ivec2& spreadSpacePos, Entity* trampler) {
    return false;
    // tramples_[key.x][key.y] = true;
    // return RemoveSpread(key, trampler);
}

bool SpreadManager::Trample(const vec3& position, Entity* trampler) {
    return false;
    // return Trample(GetKey(position));
}

int SpreadManager::Trample(const vec3& position, float radius, Entity* trampler) {
    return false;

    // int intRadius = (radius) / SpreadManager::SPREAD_DIST;
    //
    // int count = 0;
    // ivec2 origin = GetKey(position);
    // for (int x = -intRadius; x <= intRadius; x++) {
    // for (int z = -intRadius; z <= intRadius; z++) {
    //     if (sqrt(x*x + z*z) > intRadius)
    //         continue;
    //     if (Trample(origin + ivec2(x, z), trampler))
    //         count++;
    // } }
    // DEBUGLOG(intRadius);
    // return count;
}


void SpreadManager::Reset() {
    chunkIndexes_.clear();
    for (int c = 0; c < NUM_CHUNKS; c++) {
        DEBUGLOG(c);
        Chunk& chunk = chunks_[c];
        chunk.active = false;
        for (int t = 0; t < SpreadType_Num; t++)
            chunk.renderData[t].clear();

        for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            chunk.indexes[x][y] = -1; 
        }}
    }
}
