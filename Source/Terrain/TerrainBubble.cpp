#include "TerrainBubble.h"
#include "Helpers/Shared_Ease.h"
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
    node.color = vec4(0.0f, 1.0f, 0.0f, 0.5f);
    float height = terrain.GetRawHeight(position_) + 10.0f;
    node.transform.position = vec3(position_.x, height, position_.y);
    node.selected = editorTarget_.selected_;
    node.transform.scale = vec3(3.0f, 6.0f, 3.0f);
    nodes.push_back(node);
}

vec3 TerrainBubble::ETarget::GetPosition() {
    float height = bubble_->terrain_->GetRawHeight(bubble_->position_) + 10.0f;
    return vec3(bubble_->position_.x, height, bubble_->position_.y);
}

void TerrainBubble::ETarget::SetPosition(const glm::vec3 &pos) {
    bubble_->position_.x = pos.x;
    bubble_->position_.y = pos.z;
}

bool TerrainBubble::ETarget::RayHit(const Ray& ray) {
    float height = bubble_->terrain_->GetRawHeight(bubble_->position_) + 10.0f;
    return ray.HitSphere(vec3(bubble_->position_.x, height, bubble_->position_.y), 5.0f);
}

float TerrainBubble::ETarget::GetScalar(char id) {
    switch (id) {
        case 'H':
            return bubble_->height_;

        case 'R':
            return bubble_->radius_;
    }
    return 0.0;
}

void TerrainBubble::ETarget::SetScalar(char id, float value) {
    switch (id) {
        case 'H':
            bubble_->height_ = value;
            break;

        case 'R':
            bubble_->radius_ = value;
            break;
    }
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
