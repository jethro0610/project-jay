#pragma once
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include "TerrainInfluence.h"

class TerrainCurve {
public:
    static constexpr int MAX = 8;
    static constexpr int ITERATIONS = 16;
    glm::vec4 points[4];
    bool destroy_;
    #ifdef _DEBUG
    int DBG_selectedPoint_;
    #endif

    TerrainInfluence GetInfluence(glm::vec2& pos) const;
    glm::vec4 GetPosition(float t) const;

    template <const int RES>
    void WriteAffect(uint32_t affectMap[RES][RES], int index) const {
        const int HALF_RES = RES * 0.5f;
        const float WORLD_TO_TERRAIN = RES / TerrainConsts::RANGE;
        for (int i = 0; i < 65; i++) {
            float t = i / 64.0f;
            glm::vec4 bezierPos = GetPosition(t);

            int mapX = bezierPos.x * WORLD_TO_TERRAIN;
            mapX += HALF_RES;
            int mapY = bezierPos.z * WORLD_TO_TERRAIN;
            mapY += HALF_RES;
            int range = (bezierPos.w + 10.0f) * WORLD_TO_TERRAIN;
            
            int startX = std::max(mapX - range, 0);
            int endX = std::min(mapX + range, RES - 1);
            int startY = std::max(mapY - range, 0);
            int endY = std::min(mapY + range, RES - 1);

            for (int y = startY; y <= endY; y++) {
            for (int x = startX; x <= endX; x++) {
                affectMap[y][x] |= 1UL << index;
            }}
        }
    }
};
