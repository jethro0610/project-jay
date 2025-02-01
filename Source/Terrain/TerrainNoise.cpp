#include <glm/gtx/compatibility.hpp>
#include "TerrainNoise.h"
#include "Helpers/Shared_Ease.h"
#include "Terrain.h"
#include "Collision/Ray.h"
#include "Helpers/Random.h"
using namespace glm;

TerrainNoise::TerrainNoise() {
    id_ = TerrainNoise::ID;
    editorTarget_.noise_ = this;
    editorTargets_.push_back(&editorTarget_);
}

void TerrainNoise::Init(const glm::vec3& pos) {
    active_ = true;
    position_.x = pos.x;
    position_.y = pos.z;
    maxHeight_ = 30.0f;
    minHeight_ = 0.0f;
    radius_ = 20.0f;
    frequency_ = 0.5f;
    seed_ = RandomInt();
    noise_.SetSeed(seed_);
}

float TerrainNoise::GetHeight(const glm::vec2& pos) {
    float dist = distance(pos, position_);
    float t = 1.0f - dist / radius_;
    t = clamp(t, 0.0f, 1.0f);
    t = EaseQuad(t);

    vec2 samplePos = (pos - position_) * frequency_;
    float sampleVal = noise_.GetNoise(samplePos.x, samplePos.y);

    sampleVal = (sampleVal + 1.0f) * 0.5f;
    return lerp(minHeight_, maxHeight_, sampleVal) * t;
}

void TerrainNoise::WriteRenderNodes(vector_const<RenderNode, RenderNode::MAX>& nodes, Terrain& terrain) {
    RenderNode node;
    node.color = vec4(1.0f, 0.0f, 0.0f, 0.5f);
    float height = terrain.GetRawHeight(position_) + 10.0f;
    node.transform.position = vec3(position_.x, height, position_.y);
    node.selected = editorTarget_.selected_;
    node.transform.scale = vec3(3.0f, 6.0f, 3.0f);
    nodes.push_back(node);
}

vec3 TerrainNoise::ETarget::GetPosition() {
    float height = noise_->terrain_->GetRawHeight(noise_->position_) + 10.0f;
    return vec4(noise_->position_.x, height, noise_->position_.y, 0.0f);
}

void TerrainNoise::ETarget::SetPosition(const glm::vec3 &pos) {
    noise_->position_.x = pos.x;
    noise_->position_.y = pos.z;
}

bool TerrainNoise::ETarget::RayHit(const Ray& ray) {
    float height = noise_->terrain_->GetRawHeight(noise_->position_) + 10.0f;
    return ray.HitSphere(vec3(noise_->position_.x, height, noise_->position_.y), 5.0f);
}

float TerrainNoise::ETarget::GetScalar(char id) {
    switch (id) {
        case '1':
            return noise_->radius_;

        case '2':
            return noise_->maxHeight_;

        case '3':
            return noise_->frequency_;

        case '4':
            return noise_->minHeight_;
    }
    return 0.0;
}

float TerrainNoise::ETarget::GetScalarDelta(char id) {
    if (id == '1')
        return 0.01f;
    else 
        return 1.0f;
}

void TerrainNoise::ETarget::SetScalar(char id, float value) {
    switch (id) {
        case '1':
            noise_->radius_ = value;
            break;

        case '2':
            noise_->maxHeight_ = value;
            break;

        case '3':
            noise_->frequency_ = value;
            break;

        case '4':
            noise_->minHeight_ = value;
            break;
    }
}

void TerrainNoise::Save(nlohmann::json &json) {
    json["x"] = position_.x;
    json["y"] = position_.y;
    json["radius"] = radius_;
    json["max_height"] = maxHeight_;
    json["min_height"] = minHeight_;
    json["frequency"] = frequency_;
    json["seed"] = seed_;
}

void TerrainNoise::Load(const nlohmann::json &json) {
    position_.x = json["x"];
    position_.y = json["y"];
    radius_ = json["radius"];
    maxHeight_ = json["max_height"];
    minHeight_ = json["min_height"];
    frequency_ = json["frequency"];
    seed_ = json["seed"];
    noise_.SetSeed(seed_);
}
