#include "CloneMode.h"
#include "Camera/Camera.h"
#include "Platform/PC_Platform.h"
#include "Terrain/Terrain.h"
#include "Entity/EntityList.h"
#include "Entity/Entity.h"
#include "EditorTargetController.h"
using namespace glm;

CloneMode::CloneMode(EditorModeArgs args):
EditorMode(args)
{
    mouseVisibile_ = false;
    requiresTarget_ = true;
    requiresClone_ = true;
}

std::string CloneMode::GetName() {
    switch(submode_) {
        case CS_Planar:
            return "Planar Clone";

        case CS_Terrain:
            return "Terrain Aligned Clone";

        default:
            return "Clone";
    }
}

void CloneMode::SetSubmode(CloneMode::CloneSubmode submode) {
    if (submode_ == submode) return;
    submode_ = submode;
    modeText_ = GetName();
}

void CloneMode::OnStart() {
    EditorMode::OnStart();
    original_ = &target_.Get();
    deltaX_ = 0.0f;
    deltaY_ = 0.0f;

    EditorTarget* cloned = original_->Clone();
    target_.Select(cloned);
}

void CloneMode::OnEnd() {
    EditorMode::OnEnd();
    target_.Get().DoDestroy();
    target_.Select(original_);
}

void CloneMode::Update() {
    deltaX_ += platform_.deltaMouseX_ * 0.1f;
    deltaY_ -= platform_.deltaMouseY_ * 0.1f;

    if (platform_.pressedKeys_['P'])
        SetSubmode(CS_Planar);
    if (platform_.pressedKeys_['T'])
        SetSubmode(CS_Terrain);

    vec3 planarCameraForward = camera_.transform_.GetForwardVector();
    planarCameraForward.y = 0.0f;
    planarCameraForward = normalize(planarCameraForward);

    vec3 planarCameraRight = camera_.transform_.GetRightVector();
    planarCameraRight.y = 0.0f;
    planarCameraRight = normalize(planarCameraRight);

    switch (submode_) {
        case CS_Planar:
            target_.Get().SetPosition(
                original_->GetPosition() +
                planarCameraForward * deltaY_ +
                planarCameraRight * deltaX_
            );
            break;

        case CS_Terrain:
            vec3 pos =
                original_->GetPosition() +
                planarCameraForward * deltaY_ +
                planarCameraRight * deltaX_;
            pos.y = terrain_.GetHeight(pos);
            target_.Get().SetPosition(pos);
            break;
    }
}

ConfirmBehavior CloneMode::OnConfirm() {
    target_.Get().Clone();
    return CB_Stay;
}
