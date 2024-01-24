#include "TerrainBubble.h"
#include "Helpers/Ease.h"
#include <glm/gtx/compatibility.hpp>
using namespace glm;
using namespace TerrainConsts;

TerrainInfluence TerrainBubble::GetInfluence(glm::vec2& pos) const {
    vec2 bubblePos = vec2(position.x, position.z);
    float d = distance(pos, bubblePos) / position.w;

    TerrainInfluence influence;
    influence.distance = d;
    influence.height = position.y * EaseInOutQuad(1.0f - influence.distance);
    return influence;
}
