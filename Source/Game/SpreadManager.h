#pragma once
#include "../Types/Transform.h"
#include "../Game/World.h"
#include "../Resource/ResourceManager.h"
#include <set>
#include <glm.hpp>
#include <deque>
#include <unordered_set>
#include <algorithm>
#include "SpreadConstants.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/hash.hpp"

struct AddSpreadInfo {
    bool added;
    ivec2 key;
};

struct SpreadChunk {
    glm::vec3 positions[MAX_SPREAD];
    glm::ivec2 keys[MAX_SPREAD];
    uint16_t count;

    std::unordered_map<glm::ivec2, uint16_t> keysToIndex;

    SpreadChunk() {
        count = 0;
        std::fill_n(positions, MAX_SPREAD, vec3(0.0f, 0.0f, 0.0f));
        std::fill_n(keys, MAX_SPREAD, ivec2(0, 0));
    }
};

class SpreadManager {
public:
    SpreadManager(ResourceManager& resourceManager, World& world);

    ResourceManager& resourceManager_;
    World& world_;

    SpreadChunk chunks_[MAX_X_CHUNKS][MAX_Z_CHUNKS];
    std::unordered_set<glm::ivec2> dirtyChunks_;

    glm::ivec2 WorldPositionToSpreadKey(glm::vec3 position) const;
    glm::ivec2 SpreadKeyToChunk(glm::ivec2 key) const;
    bool SpreadIsActive(glm::ivec2 key) const;

    bool AddSpread(glm::ivec2 key, float height); 
    AddSpreadInfo AddSpread(glm::vec3 position); 
    AddSpreadInfo AddSpread(glm::vec3 position, int radius); 

    bool RemoveSpread(glm::ivec2 key);
    bool RemoveSpread(glm::vec3 position);
    void RemoveSpread(glm::vec3 position, int radius); 

    void UpdateRenderData_P();
};
