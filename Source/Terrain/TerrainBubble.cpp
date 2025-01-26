#include "TerrainBubble.h"
#include "Helpers/Ease.h"
#include "Terrain.h"
#include "Collision/Ray.h"
using namespace glm;

TerrainBubble::TerrainBubble() {
    id_ = TerrainBubble::ID;
    editorTarget_.bubble_ = this;
    editorTargets_.push_back(&editorTarget_);
}

void TerrainBubble::Init(const glm::vec3& pos) {
    active_ = true;
    position_.x = pos.x;
    position_.y = pos.z;
    height_ = 30.0f;
    radius_ = 20.0f;
}

float TerrainBubble::GetHeight(const vec2& pos) {
    float dist = distance(pos, position_);
    float t = EaseInOutQuad(1.0f - (dist / radius_));
    return height_ * t;
}

void TerrainBubble::WriteRenderNodes(vector_const<RenderNode, RenderNode::MAX>& nodes, Terrain& terrain) {
    RenderNode node;
    float height = terrain.GetNearestHeight(position_) + 10.0f;
    node.transform.position = vec3(position_.x, height, position_.y);
    node.selected = editorTarget_.selected_;
    node.transform.scale = vec3(3.0f, 6.0f, 3.0f);
    nodes.push_back(node);
}

vec3 TerrainBubble::ETarget::GetPosition() {
    float height = bubble_->terrain_->GetNearestHeight(bubble_->position_) + 10.0f;
    return vec3(bubble_->position_.x, height, bubble_->position_.y);
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

bool TerrainBubble::ETarget::RayHit(const Ray& ray) {
    float height = bubble_->terrain_->GetNearestHeight(bubble_->position_) + 10.0f;
    return ray.HitSphere(vec3(bubble_->position_.x, height, bubble_->position_.y), 5.0f);
}

void TerrainBubble::Save(nlohmann::json &json) {
    json["x"] = position_.x;
    json["y"] = position_.y;
    json["height"] = height_;
    json["radius"] = radius_;
}

void TerrainBubble::Load(const nlohmann::json &json) {
    position_.x = json["x"];
    position_.y = json["y"];
    height_ = json["height"];
    radius_ = json["radius"];
}
