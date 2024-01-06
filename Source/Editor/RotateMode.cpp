#include "RotateMode.h"
#include "Camera/Camera.h"
#include "Entity/EntityList.h"
#include "Platform/PC_Platform.h"
#include "EditorTarget.h"
using namespace glm;

RotateMode::RotateMode(EditorModeArgs args):
EditorMode(args) {
    mouseVisibile_ = false;
    requiresTarget_ = true;
}

std::string RotateMode::GetName() { 
    switch(submode_) {
        case RS_Free:
            return "Free Rotation";

        case RS_X:
            return "X Rotation"; 

        case RS_Y:
            return "Y Rotation";

        case RS_Z:
            return "Z Rotation"; 

        case RS_Roll:
            return "Roll Rotation"; 

        default:
            return "Rotation";
    }
};

void RotateMode::OnStart() {
    submode_ = RS_Free;
    deltaX_ = 0.0f;
    deltaY_ = 0.0f;
    fromZero_ = false;
    startRotation_ = target_.Get()->transform_.rotation;
    EditorMode::OnStart();
}

void RotateMode::OnCancel() {
    target_.Get()->transform_.rotation = startRotation_;
    EditorMode::OnCancel();
}

void RotateMode::Update() {
    EntityS* entity = target_.Get();
    if (platform_.pressedKeys_['0']) {
        deltaX_ = 0.0f;
        deltaY_ = 0.0f;
        fromZero_ = !fromZero_;
    }

    if (platform_.pressedKeys_['F'])
        SetSubmode(RS_Free);
    else if (platform_.pressedKeys_['X'])
        SetSubmode(RS_X);
    else if (platform_.pressedKeys_['Y'])
        SetSubmode(RS_Y);
    else if (platform_.pressedKeys_['Z'])
        SetSubmode(RS_Z);
    else if (platform_.pressedKeys_['R'])
        SetSubmode(RS_Roll);

    deltaX_ += platform_.deltaMouseX_ * 0.005f;
    deltaY_ += platform_.deltaMouseY_ * 0.005f;

    vec3 planarCameraForward = camera_.transform_.GetForwardVector();
    planarCameraForward.y = 0.0f;
    planarCameraForward = normalize(planarCameraForward);

    vec3 planarCameraRight = camera_.transform_.GetRightVector();
    planarCameraRight.y = 0.0f;
    planarCameraRight = normalize(planarCameraRight);

    quat referenceRotation = fromZero_ ? quat(vec3(0.0f, 0.0f, 0.0f)) : startRotation_;

    switch(submode_) {
        case RS_Free:
            entity->transform_.rotation = angleAxis(deltaX_, planarCameraForward) * angleAxis(deltaY_, planarCameraRight) * referenceRotation;
            break;

        case RS_X:
            entity->transform_.rotation = angleAxis(deltaX_, planarCameraForward) * referenceRotation;
            break;

        case RS_Y:
            entity->transform_.rotation = angleAxis(deltaX_, Transform::worldUp) * referenceRotation;
            break;

        case RS_Z:
            entity->transform_.rotation = angleAxis(deltaY_, planarCameraRight) * referenceRotation;
            break;

        case RS_Roll:
            entity->transform_.rotation = angleAxis(deltaX_, referenceRotation * Transform::worldUp) * referenceRotation;
            break;
    }
}

void RotateMode::SetSubmode(RotateSubmode submode) {
    if (submode_ == submode) return;
    submode_ = submode;
    deltaX_ = 0.0f;
    deltaY_ = 0.0f;

    modeText_ = GetName();
}
