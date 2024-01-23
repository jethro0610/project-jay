#include "TerrainCurve.h"
#include "Helpers/Ease.h"
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;
using namespace TerrainConsts;

vec4 TerrainCurve::GetPosition(float t) {
    t = clamp(t, 0.0f, 1.0f);
    float invT = 1.0f - t;

    vec4 point = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    point += invT * invT * invT * points[0];
    point += (t * 3.0f * invT * invT) * points[1];
    point += (t * t * 3.0f * invT) * points[2];
    point += t * t * t * points[3];

    return point;
}

TerrainInfluence TerrainCurve::GetInfluence(glm::vec2& pos) {
    float pivot = 0.0f;
    float nearestSqr = INFINITY;
    float nearestRadius = 0.0f;
    float nearestHeight = 0.0f;
    float notchLength = 0.25f;

    for (int i = 1; i < 4; i++) {
        float t = i * notchLength;
        vec4 bezierPos = GetPosition(t);
        float dSqr = distance2(vec2(bezierPos.x, bezierPos.z), pos);
        if (dSqr < nearestSqr) {
            nearestSqr = dSqr;
            pivot = t;
        }
    }

    for (int i = 0; i < ITERATIONS - 1; i++) {
        float start = pivot - notchLength;
        notchLength *= 0.5f;

        nearestSqr = INFINITY;
        for (int j = 0; j < 5; j++) {
            float t = start + j * notchLength;
            vec4 bezierPos = GetPosition(t);
            float dSqr = distance2(vec2(bezierPos.x, bezierPos.z), pos);
            if (dSqr < nearestSqr) {
                nearestRadius = bezierPos.w;
                nearestHeight = bezierPos.y;
                nearestSqr = dSqr;
                pivot = clamp(t, 0.0f, 1.0f);
            }
        }
    }

    TerrainInfluence influence;
    influence.distance = (sqrt(nearestSqr) / nearestRadius);
    influence.height = nearestHeight * EaseInOutQuad(1.0f - influence.distance);
    return influence;
}

void TerrainCurve::WriteAffect(uint32_t affectMap[RESOLUTION][RESOLUTION], int index) {
    for (int i = 0; i < 65; i++) {
        float t = i / 64.0f;
        vec4 bezierPos = GetPosition(t);

        int mapX = bezierPos.x * WORLD_TO_TERRAIN_SCALAR;
        mapX += HALF_RESOLUTION;
        int mapY = bezierPos.z * WORLD_TO_TERRAIN_SCALAR;
        mapY += HALF_RESOLUTION;
        int range = (bezierPos.w + 10.0f) * WORLD_TO_TERRAIN_SCALAR;
        
        int startX = max(mapX - range, 0);
        int endX = min(mapX + range, RESOLUTION - 1);
        int startY = max(mapY - range, 0);
        int endY = min(mapY + range, RESOLUTION - 1);

        for (int x = startX; x <= endX; x++) {
        for (int y = startY; y <= endY; y++) {
            affectMap[x][y] |= 1UL << index;
        }}
    }
}

void TerrainCurve::WriteAffectLow(uint32_t affectMap[RESOLUTION_LOW][RESOLUTION_LOW], int index) {
    for (int i = 0; i < 65; i++) {
        float t = i / 64.0f;
        vec4 bezierPos = GetPosition(t);

        int mapX = bezierPos.x * WORLD_TO_TERRAIN_SCALAR_LOW;
        mapX += HALF_RESOLUTION_LOW;
        int mapY = bezierPos.z * WORLD_TO_TERRAIN_SCALAR_LOW;
        mapY += HALF_RESOLUTION_LOW;
        int range = (bezierPos.w + 10.0f) * WORLD_TO_TERRAIN_SCALAR_LOW;
        
        int startX = max(mapX - range, 0);
        int endX = min(mapX + range, RESOLUTION_LOW - 1);
        int startY = max(mapY - range, 0);
        int endY = min(mapY + range, RESOLUTION_LOW - 1);

        for (int x = startX; x <= endX; x++) {
        for (int y = startY; y <= endY; y++) {
            affectMap[x][y] |= 1UL << index;
        }}
    }
}
