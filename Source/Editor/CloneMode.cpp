#include "CloneMode.h"
#include "Camera/Camera.h"
#include "Platform/PC_Platform.h"
#include "Terrain/Terrain.h"
#include "Entity/EntityListS.h"
#include "EditorTarget.h"
using namespace glm;

CloneMode::CloneMode(EditorModeArgs args):
EditorMode(args)
{
    mouseVisibile_ = false;
    requiresTarget_ = true;
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
    original_ = target_.Get();
    originalTransform_ = entities_[original_].transform_;

    EntityIDS createdEntity = entities_.CreateEntity(entities_.GetTypeID(original_));
    entities_[createdEntity].transform_ = originalTransform_;
    target_.Set(createdEntity);
}

void CloneMode::OnEnd() {
    EditorMode::OnEnd();
    entities_.DestroyEntity(target_.Get());
    target_.Set(original_);
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

    EntityS& entity = entities_[target_.Get()]; 
    switch (submode_) {
        case CS_Planar:
            entity.transform_.position =
                originalTransform_.position +
                planarCameraForward * deltaY_ +
                planarCameraRight * deltaX_;
            break;

        case CS_Terrain:
            entity.transform_.position =
                originalTransform_.position +
                planarCameraForward * deltaY_ +
                planarCameraRight * deltaX_;
            entity.transform_.position.y = terrain_.GetHeight(vec2(entity.transform_.position.x, entity.transform_.position.z));
            break;
    }
}

ConfirmBehavior CloneMode::OnConfirm() {
    EntityIDS createdEntity = entities_.CreateEntity(entities_.GetTypeID(target_.Get()));
    entities_[createdEntity].transform_ = entities_[target_.Get()].transform_;

    return CB_Stay;
}
