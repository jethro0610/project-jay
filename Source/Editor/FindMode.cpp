#include "FindMode.h"
#include "Camera/Camera.h"
#include "Entity/EntityManager.h"
#include "Components/TransformComponent.h"
#include "EditorNotification.h"
#include "EditorTextInput.h"
#include "EditorTarget.h"

FindMode::FindMode(EditorModeArgs args):
EditorMode(args) {
    requiresTarget_ = false;
    requiresLevel_ = true;
    mouseVisibile_ = false;
    ctrl_ = false;
}

void FindMode::OnStart() {
    lastFind_ = 0;
    textInput_.SetLabel("e_"); 
    EditorMode::OnStart();
}

void FindMode::OnEnd() {
    textInput_.Clear();
}

void FindMode::Update() {
    textInput_.ReadInput();
}

ConfirmBehavior FindMode::OnConfirm() {
    TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();
    std::string entityName = "e_" + textInput_.Get();

    bool found = false;
    for (int i = 0; i < MAX_ENTITIES; i++) {
        int e = (i + lastFind_ + 1) % MAX_ENTITIES;
        const Entity& entity = entityManager_.entities_[e];

        if (!entity.alive_) continue;
        
        if (entity.DBG_name == entityName) {
            lastFind_ = e;
            target_.Set(e);
            camera_.transform_.position = 
                transformComponent.transform[e].position - 
                camera_.transform_.GetForwardVector() * 35.0f;
            found = true;
            break;
        }
    }

    if (!found)
        notificaiton_.Set(entityName + " not found");

    return CB_Stay;
}
