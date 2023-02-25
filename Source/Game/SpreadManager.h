#pragma once
#include "../Types/Transform.h"
#include "../Game/World.h"
#include "../Resource/ResourceManager.h"
#include <unordered_map>
#include <glm.hpp>
#include <unordered_set>
#include "SpreadConstants.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/hash.hpp"

struct AddSpreadInfo {
    bool added;
    ivec2 key;
};

struct SpreadChunk {
    std::unordered_map<glm::ivec2, uint16_t> keys;
    uint16_t count = 0;
    glm::vec3 positions[MAX_SPREAD];
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
    void AddSpread(glm::vec3 position, int radius); 
    void UpdateRenderData_P();
};
