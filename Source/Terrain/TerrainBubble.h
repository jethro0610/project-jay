#pragma once
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include "TerrainInfluence.h"
#include "Shared_TerrainConstants.h"

class TerrainBubble {
public:
    static constexpr int MAX = 16;
    glm::vec4 position;
    bool destroy_;
    #ifdef _DEBUG
    bool DBG_selected_;
    #endif

    TerrainInfluence GetInfluence(glm::vec2& pos, float offset = 0.0f) const;

    template<const int RES>
    void WriteAffect(uint32_t affectMap[RES][RES], int index) const {
        const int HALF_RES = RES * 0.5f;
        const float WORLD_TO_TERRAIN = RES / TerrainConsts::RANGE;
        int mapX = position.x * WORLD_TO_TERRAIN;
        mapX += HALF_RES;
        int mapY = position.z * WORLD_TO_TERRAIN;
        mapY += HALF_RES;
        int range = (position.w + 10.0f) * WORLD_TO_TERRAIN;
        
        int startX = std::max(mapX - range, 0);
        int endX = std::min(mapX + range, RES - 1);
        int startY = std::max(mapY - range, 0);
        int endY = std::min(mapY + range, RES - 1);

        for (int y = startY; y <= endY; y++) {
        for (int x = startX; x <= endX; x++) {
            affectMap[y][x] |= 1UL << index;
        }}
    }
};
