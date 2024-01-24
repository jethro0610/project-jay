#include "EditorTarget.h"
#include "Entity/Entity.h"
#include "Terrain/TerrainBubble.h"
#include "Terrain/TerrainCurve.h"

EditorTarget::EditorTarget() {
    entity_ = nullptr;
    bubble_ = nullptr;
    curve_ = nullptr;
    curvePoint_ = -1;
}

void EditorTarget::Untarget() {
    if (entity_ != nullptr)
        entity_->DBG_selected_ = false;
    if (bubble_ != nullptr)
        bubble_->DBG_selected_ = false;
    if (curve_ != nullptr)
        curve_->DBG_selectedPoint_ = -1;

    entity_ = nullptr;
    bubble_ = nullptr;
    curve_ = nullptr;
    curvePoint_ = -1;

    name_ = "";
}

void EditorTarget::Destroy() {
    if (entity_ != nullptr)
        entity_->destroy_ = true;
    else if (bubble_ != nullptr)
        bubble_->destroy_ = true;
    else if (curve_ != nullptr)
        curve_->destroy_ = true;

    Untarget();
}

void EditorTarget::SetEntity(Entity* target) {
    assert(target != nullptr);
    Untarget();
    entity_ = target;

    entity_->DBG_selected_ = true;
    name_ = std::string(entity_->DBG_name_) + '(' + std::to_string(target->DBG_index_) + ')';
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

glm::vec3 EditorTarget::GetPosition() {
    if (entity_ != nullptr)
        return entity_->transform_.position;
    else if (bubble_ != nullptr)
        return bubble_->position;
    else if (curve_ != nullptr)
        return curve_->points[curvePoint_];
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
}

glm::vec3 EditorTarget::GetScale() {
    if (entity_ != nullptr)
        return entity_->transform_.scale;
    else if (bubble_ != nullptr)
        return glm::vec3(bubble_->position.w);
    else if (curve_ != nullptr)
        return glm::vec3(curve_->points[curvePoint_].w);
    else
        return glm::vec3(0.0f);
}

void EditorTarget::SetScale(glm::vec3 scale) {
    if (entity_ != nullptr)
        entity_->transform_.scale = scale;
    else if (bubble_ != nullptr)
        bubble_->position.w = scale.x;
    else if (curve_ != nullptr)
        curve_->points[curvePoint_].w = scale.x;
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
