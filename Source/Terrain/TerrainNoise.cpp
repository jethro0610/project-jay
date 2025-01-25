#include <glm/gtx/compatibility.hpp>
#include "TerrainNoise.h"
#include "Helpers/Ease.h"
#include "Terrain.h"
#include "Collision/Ray.h"
using namespace glm;

TerrainNoise::TerrainNoise() {
    editorTarget_.noise_ = this;
}

TerrainNoise::TerrainNoise(
    int seed, 
    glm::vec2 position, 
    float radius, 
    float minHeight, 
    float maxHeight, 
    float frequency
) :
    seed_(seed),
    noise_(seed),
    position_(position),
    radius_(radius),
    minHeight_(minHeight),
    maxHeight_(maxHeight),
    frequency_(frequency)
{
    editorTarget_.noise_ = this;
}

bool TerrainNoise::InRange(const glm::vec2& pos) {
    float dist = glm::distance2(pos, position_);
    return dist <= radius_ * radius_;
}

float TerrainNoise::GetHeight(const glm::vec2& pos) {
    float dist = distance(pos, position_);
    float t = EaseInOutQuad(1.0f - (dist / radius_));
    vec2 samplePos = (pos - position_) * frequency_;
    float sampleVal = noise_.GetNoise(samplePos.x, samplePos.y);

    sampleVal = (sampleVal + 1.0f) * 0.5f;
    return lerp(minHeight_, maxHeight_, sampleVal) * t;
}

vec3 TerrainNoise::ETarget::GetPosition() {
    return vec4(noise_->position_.x, 0.0, noise_->position_.y, 0.0f);
}

void TerrainNoise::ETarget::SetPosition(const glm::vec3 &pos) {
    noise_->position_.x = pos.x;
    noise_->position_.y = pos.z;
}

glm::vec4 TerrainNoise::ETarget::GetScale() {
    return vec4(noise_->radius_, noise_->maxHeight_, noise_->frequency_, noise_->minHeight_);
};

void TerrainNoise::ETarget::SetScale(const glm::vec4& ref, const glm::vec4& delta) {
    noise_->radius_ = ref.x + delta.x;
    noise_->maxHeight_ = ref.y + delta.y;
    noise_->frequency_ = ref.z + delta.z;
    noise_->minHeight_ = ref.w + delta.w;
};

bool TerrainNoise::ETarget::RayHit(const Ray& ray, Terrain& terrain) {
    return ray.HitSphere(vec3(noise_->position_.x, terrain.GetHeight(noise_->position_), noise_->position_.y), 5.0f);
}
