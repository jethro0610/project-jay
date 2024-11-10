#include "TerrainNoise.h"
#include "Helpers/Ease.h"
#include <glm/gtx/compatibility.hpp>
using namespace glm;
using namespace TerrainConsts;

float TerrainNoise::GetHeight(glm::vec2& pos) const {
    float dist = distance(pos, position_);
    float t = EaseInOutQuad(1.0f - (dist / radius_));
    vec2 samplePos = (pos - position_) * frequency_;
    float sampleVal = noise_.GetNoise(samplePos.x, samplePos.y);

    sampleVal = (sampleVal + 1.0f) * 0.5f;
    return lerp(minHeight_, maxHeight_, sampleVal) * t;
}

bool TerrainNoise::InRange(glm::vec2& pos) const {
    float dist = distance(pos, position_);
    return dist <= radius_;
}
