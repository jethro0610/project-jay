#pragma once
#include <set>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>
#include <deque>
#include <unordered_set>
#include <algorithm>
#include "../../Constants/WorldConstants.h"
#include "../../Constants/SpreadConstants.h"
#include "../../Types/FixedVector.h"
#include "../../Types/EntityTypes.h"
class World;
class ResourceManager;
class SeedManager;
class TransformComponent;

struct AddSpreadInfo {
    uint16_t count;
    glm::ivec2 key;
};

struct SpreadChunk {
    glm::vec3 positions[MAX_SPREAD];
    glm::ivec2 keys[MAX_SPREAD];
    uint16_t count;

    std::unordered_map<glm::ivec2, uint16_t> keysToIndex;

    SpreadChunk() {
        count = 0;
        std::fill_n(positions, MAX_SPREAD, glm::vec3(0.0f, 0.0f, 0.0f));
        std::fill_n(keys, MAX_SPREAD, glm::ivec2(0, 0));
    }
};

class SpreadManager {
public:
    SpreadChunk spreadChunks_[MAX_X_CHUNKS][MAX_Z_CHUNKS];
    SpreadManager(
        ResourceManager& resourceManager,
        SeedManager& seedManager,
        World& world
    );
    SpreadManager(const SpreadManager&) = delete;

    glm::ivec2 WorldPositionToSpreadKey(glm::vec3 position) const;
    glm::ivec2 SpreadKeyToChunk(glm::ivec2 key) const;
    bool SpreadIsActive(glm::ivec2 key) const;

    bool AddSpread(glm::ivec2 key, float height); 
    bool AddSpread(glm::vec3 position); 
    AddSpreadInfo AddSpread(glm::vec3 position, int radius, uint16_t amount = UINT16_MAX);

    bool RemoveSpread(glm::ivec2 key);
    bool RemoveSpread(glm::vec3 position);
    uint16_t RemoveSpread(glm::vec3 position, int16_t radius); 
    void UpdateRenderData_P();

private:
    ResourceManager& resourceManager_;
    World& world_;
    SeedManager& seedManager_;
    std::unordered_set<glm::ivec2> dirtyChunks_;
    FixedVector<glm::ivec2, 8192> viableAddKeys_; // Making this a member variable so its not reallocated every call
};
