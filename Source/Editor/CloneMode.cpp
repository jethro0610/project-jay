#include "CloneMode.h"
#include "Camera/Camera.h"
#include "Platform/PC_Platform.h"
#include "Terrain/Terrain.h"
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
    TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();

    EditorMode::OnStart();
    spawnPromise_.spawned = false;
    original_ = target_.Get();
    originalDescription_ = resourceManager_.GetEntityDescription(entityManager_.entities_[original_].DBG_name);
    originalTransform_ = transformComponent.transform[original_];

    entityManager_.spawner_.Spawn(originalDescription_, originalTransform_, &spawnPromise_);
}

void CloneMode::OnEnd() {
    EditorMode::OnEnd();
    entityManager_.destroyList_.push_back({target_.Get(), false});
    target_.Set(original_);
}

void CloneMode::Update() {
    deltaX_ += platform_.deltaMouseX_ * 0.1f;
    deltaY_ -= platform_.deltaMouseY_ * 0.1f;

    if (platform_.pressedKeys_['P'])
        SetSubmode(CS_Planar);
    if (platform_.pressedKeys_['T'])
        SetSubmode(CS_Terrain);

    if (spawnPromise_.waiting) {
        if (!spawnPromise_.spawned) return;
        target_.Set(spawnPromise_.id);
        spawnPromise_.waiting = false;
        spawnPromise_.spawned = false;
    }
    TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();
    Transform& transform = transformComponent.transform[target_.Get()];
    
    vec3 planarCameraForward = camera_.transform_.GetForwardVector();
    planarCameraForward.y = 0.0f;
    planarCameraForward = normalize(planarCameraForward);

    vec3 planarCameraRight = camera_.transform_.GetRightVector();
    planarCameraRight.y = 0.0f;
    planarCameraRight = normalize(planarCameraRight);

    switch (submode_) {
        case CS_Planar:
            transform.position =
                originalTransform_.position +
                planarCameraForward * deltaY_ +
                planarCameraRight * deltaX_;
            break;

        case CS_Terrain:
            transform.position =
                originalTransform_.position +
                planarCameraForward * deltaY_ +
                planarCameraRight * deltaX_;
            transform.position.y = terrain_.GetHeight(vec2(transform.position.x, transform.position.z));
            break;
    }
}

ConfirmBehavior CloneMode::OnConfirm() {
    TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();
    Transform& transform = transformComponent.transform[target_.Get()];
    entityManager_.spawner_.Spawn(originalDescription_, transform);

    return CB_Stay;
}
