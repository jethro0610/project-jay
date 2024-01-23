#pragma once
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include "TerrainInfluence.h"

class TerrainCurve {
public:
    static constexpr int MAX = 8;
    static constexpr int ITERATIONS = 10;
    glm::vec4 points[4];
    #ifdef _DEBUG
    int DBG_selectedPoint_;
    #endif

    TerrainInfluence GetInfluence(glm::vec2& pos);
    glm::vec4 GetPosition(float t);
    void WriteAffect(uint32_t affectMap[TerrainConsts::RESOLUTION][TerrainConsts::RESOLUTION], int index);
    void WriteAffectLow(uint32_t affectMap[TerrainConsts::RESOLUTION_LOW][TerrainConsts::RESOLUTION_LOW], int index);
};
