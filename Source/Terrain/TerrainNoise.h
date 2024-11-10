#pragma once
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <math.h>
#include <FastNoiseLite.h>
#include "Shared_TerrainConstants.h"

class TerrainNoise {
public:
    static constexpr int MAX = 16;
    FastNoiseLite noise_;
    glm::vec2 position_;
    float radius_;
    float minHeight_;
    float maxHeight_;
    bool destroy_;
    #ifdef _DEBUG
    bool DBG_selected_;
    #endif

    float GetHeight(glm::vec2& pos) const;
    bool InRange(glm::vec2& pos) const;
};
