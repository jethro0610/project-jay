#include "ScaleMode.h"
#include "Platform/PC_Platform.h"
#include "EditorTargetController.h"
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

        case SS_X:
            return "X Scale";

        case SS_Y:
            return "Y Scale";

        case SS_Z:
            return "Z Scale";

        case SS_W:
            return "W Scale";

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
    target_.SetScale(startScale_, vec4(0.0f));
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

    if (platform_.pressedKeys_['P'])
        SetSubmode(SS_Planar);
    else if (platform_.pressedKeys_['V'])
        SetSubmode(SS_Vertical);
    else if (platform_.pressedKeys_['U'])
        SetSubmode(SS_Uniform);
    else if (platform_.pressedKeys_['X'])
        SetSubmode(SS_X);
    else if (platform_.pressedKeys_['Y'])
        SetSubmode(SS_Y);
    else if (platform_.pressedKeys_['Z'])
        SetSubmode(SS_Z);
    else if (platform_.pressedKeys_['W'])
        SetSubmode(SS_W);

    vec4 referenceScale = fromZero_ ? vec4(1.0f) : startScale_;
    vec4 deltaScale = vec4(0.0f);

    switch (submode_) {
        case SS_Uniform: {
            deltaScale.x += delta;
            deltaScale.y += delta;
            deltaScale.z += delta;
            break;
        }

        case SS_Planar:
            deltaScale.x += delta;
            deltaScale.z += delta;
            break;

        case SS_Vertical:
            deltaScale.y += delta;
            break;

        case SS_X:
            deltaScale.x += delta;
            break;

        case SS_Y:
            deltaScale.y += delta;
            break;

        case SS_Z:
            deltaScale.z += delta;
            break;

        case SS_W:
            deltaScale.w += delta;
            break;
    }
    target_.SetScale(referenceScale, deltaScale);
}
