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
    height_ = 50.0f;
    radius_ = 50.0f;
    inPower_ = 2.0f;
    outPower_ = 2.4f;
}

float TerrainBubble::GetHeight(const vec2& pos) {
    float dist = distance(pos, position_);
    float t = 1.0f - dist / radius_;
    t = clamp(t, 0.0f, 1.0f);
    if (customEasing_)
        t = EaseInOut(t, inPower_, outPower_);
    else
        t = EaseQuad(t);
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
        case '1':
            return bubble_->radius_;

        case '2':
            return bubble_->height_;

        case '3':
            return bubble_->inPower_;

        case '4':
            return bubble_->outPower_;
    }
    return 0.0;
}

float TerrainBubble::ETarget::GetScalarDelta(char id) {
    if (id == '3' || id == '4')
        return 0.1f;
    else
        return 1.0f;
}

void TerrainBubble::ETarget::SetScalar(char id, float value) {
    switch (id) {
        case '1':
            bubble_->radius_ = value;
            break;

        case '2':
            bubble_->height_ = value;
            break;

        case '3':
            bubble_->inPower_ = value;
            break;

        case '4':
            bubble_->outPower_ = value;
            break;
    }
}

void TerrainBubble::ETarget::SetFlag(char id) {
    switch(id) {
        case ',':
            bubble_->customEasing_ = false;
            break;

        case '.':
            bubble_->customEasing_ = true;
            break;
    }
}

void TerrainBubble::Save(nlohmann::json &json) {
    json["x"] = position_.x;
    json["y"] = position_.y;
    json["height"] = height_;
    json["radius"] = radius_;

    json["in_power"] = inPower_;
    json["out_power"] = outPower_;
    json["custom_easing"] = customEasing_;
}

void TerrainBubble::Load(const nlohmann::json &json) {
    position_.x = json["x"];
    position_.y = json["y"];
    height_ = json["height"];
    radius_ = json["radius"];

    if (json.contains("in_power"))
        inPower_ = json["in_power"];
    if (json.contains("out_power"))
        outPower_ = json["out_power"];
    if (json.contains("custom_easing"))
        customEasing_ = json["custom_easing"];
}
