#pragma once
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include "TerrainInfluence.h"
#include "Shared_TerrainConstants.h"

class TerrainBubble {
public:
    static constexpr int MAX = 8;
    glm::vec4 position;
    #ifdef _DEBUG
    bool DBG_selected_;
    #endif

    TerrainInfluence GetInfluence(glm::vec2& pos);
    void WriteAffect(uint32_t affectMap[TerrainConsts::RESOLUTION][TerrainConsts::RESOLUTION], int index);
    void WriteAffectLow(uint32_t affectMap[TerrainConsts::RESOLUTION_LOW][TerrainConsts::RESOLUTION_LOW], int index);
};
