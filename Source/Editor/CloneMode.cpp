#include "CloneMode.h"
#include "Camera/Camera.h"
#include "Platform/PC_Platform.h"
#include "Terrain/Terrain.h"
#include "Entity/EntityList.h"
#include "Entity/Entity.h"
#include "EditorTarget.h"
using namespace glm;

CloneMode::CloneMode(EditorModeArgs args):
EditorMode(args)
{
    mouseVisibile_ = false;
    requiresTarget_ = true;
    requiresEntity_ = true;
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
    original_ = target_.GetEntity();
    originalTransform_ = original_->transform_;
    deltaX_ = 0.0f;
    deltaY_ = 0.0f;

    Entity& createdEntity = entities_.CreateEntity(original_->typeId_, Transform(), true);
    createdEntity.transform_ = originalTransform_;
    createdEntity.CopyProperties(original_);
    target_.SetEntity(&createdEntity);
}

void CloneMode::OnEnd() {
    EditorMode::OnEnd();
    target_.GetEntity()->destroy_ = true;
    target_.SetEntity(original_);
}

void CloneMode::Update() {
    Entity* entity = target_.GetEntity(); 
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
            entity->transform_.position =
                originalTransform_.position +
                planarCameraForward * deltaY_ +
                planarCameraRight * deltaX_;
            break;

        case CS_Terrain:
            entity->transform_.position =
                originalTransform_.position +
                planarCameraForward * deltaY_ +
                planarCameraRight * deltaX_;
            entity->transform_.position.y = terrain_.GetHeight(vec2(entity->transform_.position.x, entity->transform_.position.z));
            break;
    }
}

ConfirmBehavior CloneMode::OnConfirm() {
    Entity& createdEntity = entities_.CreateEntity(target_.GetEntity()->typeId_, Transform(), true);
    createdEntity.transform_ = target_.GetEntity()->transform_;
    createdEntity.CopyProperties(original_);

    return CB_Stay;
}
