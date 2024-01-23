#include "TerrainBubble.h"
#include "Helpers/Ease.h"
#include <glm/gtx/compatibility.hpp>
using namespace glm;
using namespace TerrainConsts;

TerrainInfluence TerrainBubble::GetInfluence(glm::vec2& pos) {
    vec2 bubblePos = vec2(position.x, position.z);
    float d = distance(pos, bubblePos) / position.w;

    TerrainInfluence influence;
    influence.distance = d;
    influence.height = position.y * EaseInOutQuad(1.0f - influence.distance);
    return influence;
}

void TerrainBubble::WriteAffect(uint32_t affectMap[RESOLUTION][RESOLUTION], int index) {
    int mapX = position.x * WORLD_TO_TERRAIN_SCALAR;
    mapX += HALF_RESOLUTION;
    int mapY = position.z * WORLD_TO_TERRAIN_SCALAR;
    mapY += HALF_RESOLUTION;
    int range = (position.w + 10.0f) * WORLD_TO_TERRAIN_SCALAR;
    
    int startX = max(mapX - range, 0);
    int endX = min(mapX + range, RESOLUTION - 1);
    int startY = max(mapY - range, 0);
    int endY = min(mapY + range, RESOLUTION - 1);

    for (int x = startX; x <= endX; x++) {
    for (int y = startY; y <= endY; y++) {
        affectMap[x][y] |= 1UL << index;
    }}
}

void TerrainBubble::WriteAffectLow(uint32_t affectMap[RESOLUTION_LOW][RESOLUTION_LOW], int index) {
    int mapX = position.x * WORLD_TO_TERRAIN_SCALAR_LOW;
    mapX += HALF_RESOLUTION_LOW;
    int mapY = position.z * WORLD_TO_TERRAIN_SCALAR_LOW;
    mapY += HALF_RESOLUTION_LOW;
    int range = (position.w + 10.0f) * WORLD_TO_TERRAIN_SCALAR_LOW;
    
    int startX = max(mapX - range, 0);
    int endX = min(mapX + range, RESOLUTION_LOW - 1);
    int startY = max(mapY - range, 0);
    int endY = min(mapY + range, RESOLUTION_LOW - 1);

    for (int x = startX; x <= endX; x++) {
    for (int y = startY; y <= endY; y++) {
        affectMap[x][y] |= 1UL << index;
    }}
}
