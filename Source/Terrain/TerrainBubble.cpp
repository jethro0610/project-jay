#include "TerrainBubble.h"
#include "Helpers/Ease.h"
#include "Terrain.h"
#include "Collision/Ray.h"
using namespace glm;

TerrainBubble::TerrainBubble() {
    editorTarget_.bubble_ = this;
}

TerrainBubble::TerrainBubble(
    glm::vec2 position, 
    float radius, 
    float height
) :
    position_(position),
    radius_(radius),
    height_(height)
{
    editorTarget_.bubble_ = this;    
}

bool TerrainBubble::InRange(const glm::vec2& pos) {
    float dist = glm::distance2(pos, position_);
    return dist <= radius_ * radius_;
}

float TerrainBubble::GetHeight(const vec2& pos) {
    float dist = distance(pos, position_);
    float t = EaseInOutQuad(1.0f - (dist / radius_));
    return height_ * t;
}

vec3 TerrainBubble::ETarget::GetPosition() {
    return vec3(bubble_->position_.x, 0.0, bubble_->position_.y);
}

void TerrainBubble::ETarget::SetPosition(const glm::vec3 &pos) {
    bubble_->position_.x = pos.x;
    bubble_->position_.y = pos.z;
}

glm::vec4 TerrainBubble::ETarget::GetScale() {
    return vec4(bubble_->radius_, bubble_->height_, 0.0f, 0.0f);
};

void TerrainBubble::ETarget::SetScale(const glm::vec4& ref, const glm::vec4& delta) {
    bubble_->radius_ = ref.x + delta.x;
    bubble_->height_ = ref.y + delta.y;
};

bool TerrainBubble::ETarget::RayHit(const Ray& ray, Terrain& terrain) {
    return ray.HitSphere(vec3(bubble_->position_.x, terrain.GetHeight(bubble_->position_), bubble_->position_.y), 5.0f);
}
