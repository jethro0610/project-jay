#include "EditorTarget.h"
#include "Entity/Entity.h"
#include "Terrain/TerrainBubble.h"
#include "Terrain/TerrainCurve.h"
#include "Terrain/TerrainNoise.h"

EditorTarget::EditorTarget() {
    entity_ = nullptr;
    bubble_ = nullptr;
    curve_ = nullptr;
    noise_ = nullptr;
    curvePoint_ = -1;
}

void EditorTarget::Untarget() {
    if (entity_ != nullptr)
        entity_->DBG_selected_ = false;
    if (bubble_ != nullptr)
        bubble_->DBG_selected_ = false;
    if (curve_ != nullptr)
        curve_->DBG_selectedPoint_ = -1;
    if (noise_ != nullptr)
        noise_->DBG_selected_ = false;

    entity_ = nullptr;
    bubble_ = nullptr;
    curve_ = nullptr;
    curvePoint_ = -1;
    noise_ = nullptr;

    name_ = "";
}

void EditorTarget::Destroy() {
    if (entity_ != nullptr)
        entity_->destroy_ = true;
    else if (bubble_ != nullptr)
        bubble_->destroy_ = true;
    else if (curve_ != nullptr)
        curve_->destroy_ = true;
    else if (noise_ != nullptr)
        noise_->destroy_ = true;

    Untarget();
}

void EditorTarget::SetEntity(Entity* target) {
    assert(target != nullptr);
    Untarget();
    entity_ = target;

    entity_->DBG_selected_ = true;
    name_ = std::string(entity_->GetName()) + '(' + std::to_string(target->DBG_index_) + ')';
}

void EditorTarget::SetBubble(TerrainBubble* target) {
    assert(target != nullptr);
    Untarget();
    bubble_ = target;

    bubble_->DBG_selected_ = true;
    name_ = "e_bubble";
}

void EditorTarget::SetCurve(TerrainCurve* target, int point) {
    assert(target != nullptr);
    Untarget();
    curve_ = target;
    curvePoint_ = point;

    curve_->DBG_selectedPoint_ = point;
    name_ = "e_curve";
}

void EditorTarget::SetNoise(TerrainNoise* target) {
    assert(target != nullptr);
    Untarget();
    noise_ = target;

    noise_->DBG_selected_ = true;
    name_ = "e_noise";
}

glm::vec3 EditorTarget::GetPosition() {
    if (entity_ != nullptr)
        return entity_->transform_.position;
    else if (bubble_ != nullptr)
        return bubble_->position;
    else if (curve_ != nullptr)
        return curve_->points[curvePoint_];
    else if (noise_ != nullptr)
        return glm::vec3(noise_->position_.x, 0.0f, noise_->position_.y);
    else
        return glm::vec3(0.0f);
}

void EditorTarget::SetPosition(glm::vec3 pos) {
    if (entity_ != nullptr)
        entity_->transform_.position = pos;
    else if (bubble_ != nullptr)
        bubble_->position = glm::vec4(pos, bubble_->position.w);
    else if (curve_ != nullptr)
        curve_->points[curvePoint_] = glm::vec4(pos, curve_->points[curvePoint_].w);
    else if (noise_ != nullptr)
        noise_->position_ = glm::vec2(pos.x, pos.z);
}

glm::vec4 EditorTarget::GetScale() {
    if (entity_ != nullptr)
        return glm::vec4(entity_->transform_.scale, 0.0f);
    else if (bubble_ != nullptr)
        return glm::vec4(bubble_->position.w);
    else if (curve_ != nullptr)
        return glm::vec4(curve_->points[curvePoint_].w);
    else if (noise_!= nullptr)
        return glm::vec4(noise_->radius_, noise_->maxHeight_, noise_->frequency_, noise_->minHeight_);
    else
        return glm::vec4(0.0f);
}

void EditorTarget::SetScale(glm::vec4 reference, glm::vec4 delta) {
    if (entity_ != nullptr)
        entity_->transform_.scale = reference + delta;
    else if (bubble_ != nullptr)
        bubble_->position.w = reference.x + delta.x;
    else if (curve_ != nullptr)
        curve_->points[curvePoint_].w = reference.x + delta.x;
    else if (noise_ != nullptr) {
        noise_->radius_ = reference.x + delta.x;
        noise_->minHeight_ = reference.w + delta.w;
        noise_->maxHeight_ = reference.y + delta.y;
        noise_->frequency_ = reference.z + delta.z * 0.01f;
    }
}

glm::quat EditorTarget::GetRotation() {
    if (entity_ != nullptr)
        return entity_->transform_.rotation;
    else
        return glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
}

void EditorTarget::SetRotation(glm::quat rotation) {
    if (entity_ != nullptr)
        entity_->transform_.rotation = rotation;
}
