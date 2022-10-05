#pragma once
#include "../Types/Transform.h"
#include <unordered_map>
#include <glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/hash.hpp"

const uint16_t MAX_SPREAD = 8192;

class SpreadManager {
public:
    SpreadManager();

    std::unordered_map<glm::ivec2, uint16_t> keys_; 
    Transform transforms_[MAX_SPREAD]; 
    uint16_t count_;

    void AddSpread(glm::ivec2 key); 
};
