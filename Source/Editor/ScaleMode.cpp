#include "ScaleMode.h"
using namespace glm;

ScaleMode::ScaleMode(EditorModeArgs args):
EditorMode(args) 
{
    mouseVisibile_ = false;
    requiresTarget_ = true;
}

std::string ScaleMode::GetName() { 
    switch (submode_) {
        case SS_Uniform:
            return "Uniform Scale";

        case SS_Planar:
            return "Planar Scale";

        case SS_Vertical:
            return "Vertical Scale";

        default:
            return "Scale";
    }
}

void ScaleMode::OnStart() {
    TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();
    Transform& transform = transformComponent.transform[target_.Get()];

    deltaX_ = 0.0f;
    deltaY_ = 0.0f;
    submode_ = SS_Uniform;
    startScale_ = transform.scale;

    EditorMode::OnStart();
}

void ScaleMode::OnCancel() {
    TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();
    Transform& transform = transformComponent.transform[target_.Get()];
    transform.scale = startScale_;
}

void ScaleMode::SetSubmode(ScaleSubmode submode) {
    if (submode_ == submode) return;
    deltaX_ = 0.0f;
    deltaY_ = 0.0f;
    submode_ = submode;
    modeText_ = GetName();
}

void ScaleMode::Update() {
    deltaX_ += platform_.deltaMouseX_ * 0.1f;
    deltaY_ -= platform_.deltaMouseY_ * 0.1f;
    float delta = deltaX_ + deltaY_;

    TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();
    Transform& transform = transformComponent.transform[target_.Get()];

    if (platform_.pressedKeys_['P'])
        SetSubmode(SS_Planar);
    else if (platform_.pressedKeys_['V'])
        SetSubmode(SS_Vertical);
    if (platform_.pressedKeys_['U'])
        SetSubmode(SS_Uniform);

    switch (submode_) {
        case SS_Uniform:
            transform.scale.x = startScale_.x + delta;
            transform.scale.y = startScale_.y + delta;
            transform.scale.z = startScale_.z + delta;
            break;

        case SS_Planar:
            transform.scale.x = startScale_.x + delta;
            transform.scale.y = startScale_.y;
            transform.scale.z = startScale_.x + delta;
            break;

        case SS_Vertical:
            transform.scale.x = startScale_.x;
            transform.scale.y = startScale_.y + delta;
            transform.scale.z = startScale_.z;
            break;
    }
}
