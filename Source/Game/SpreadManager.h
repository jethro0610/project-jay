#pragma once
#include "../Types/Transform.h"
#include "../Game/World.h"
#include "../Resource/ResourceManager.h"
#include <unordered_map>
#include <glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/hash.hpp"

class SpreadManager {
public:
    SpreadManager(ResourceManager* resourceManager, World* world);

    ResourceManager* resourceManager_;
    World* world_;

    std::unordered_map<glm::ivec2, uint16_t> keys_; 
    uint16_t count_;
    glm::vec3 positions_[MAX_INSTANCES];
    bool dirty_;

    bool AddSpread(glm::ivec2 key, float height); 
    void UpdateRenderData_P();
};
