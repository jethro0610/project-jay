#include "ScaleMode.h"
#include "Entity/EntityList.h"
#include "Platform/PC_Platform.h"
#include "EditorTarget.h"
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
    deltaX_ = 0.0f;
    deltaY_ = 0.0f;
    submode_ = SS_Uniform;
    fromZero_ = false;
    startScale_ = target_.GetScale();

    EditorMode::OnStart();
}

void ScaleMode::OnCancel() {
    target_.SetScale(startScale_);
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

    if (platform_.pressedKeys_['0']) {
        deltaX_ = 0.0f;
        deltaY_ = 0.0f;
        fromZero_ = !fromZero_;
    }

    if (target_.IsEntity()) {
        if (platform_.pressedKeys_['P'])
            SetSubmode(SS_Planar);
        else if (platform_.pressedKeys_['V'])
            SetSubmode(SS_Vertical);
        else if (platform_.pressedKeys_['U'])
            SetSubmode(SS_Uniform);
    }

    vec3 referenceScale = fromZero_ ? vec3(1.0f) : startScale_;

    switch (submode_) {
        case SS_Uniform: {
            referenceScale.x += delta;
            referenceScale.y += delta;
            referenceScale.z += delta;
            break;
        }

        case SS_Planar:
            referenceScale.x += delta;
            referenceScale.x += delta;
            break;

        case SS_Vertical:
            referenceScale.y += delta;
            break;
    }
    target_.SetScale(referenceScale);
}
