#include "TerrainNoise.h"
#include "Helpers/Ease.h"
#include <glm/gtx/compatibility.hpp>
using namespace glm;
using namespace TerrainConsts;

float TerrainNoise::GetHeight(glm::vec2& pos) const {
    float dist = distance(pos, position_);
    float t = EaseInOutQuad(1.0f - (dist / radius_));
    return noise_.GetNoise(pos.x * 0.25f, pos.y * 0.25f) * 50.0f * t;
}

bool TerrainNoise::InRange(glm::vec2& pos) const {
    float dist = distance(pos, position_);
    return dist <= radius_;
}
