#include "FindMode.h"
#include "Camera/Camera.h"
#include "Entity/EntityList.h"
#include "EditorNotification.h"
#include "EditorTextInput.h"
#include "EditorTargetController.h"
#include "Entity/Entity.h"

FindMode::FindMode(EditorModeArgs args):
EditorMode(args) {
    requiresTarget_ = false;
    requiresLevel_ = true;
    mouseVisibile_ = false;
    ctrl_ = true;
}

void FindMode::OnStart() {
    lastFindIndex_ = 0;
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
    std::string entityName = "e_" + textInput_.Get();

    bool found = false;
    for (int i = 0; i < targets_.size(); i++) {
        int t = (i + lastFindIndex_ + 1) % targets_.size();
        if (!targets_[t]->Selectable()) continue;
        
        if (targets_[t]->GetName() == entityName) {
            target_.Select(targets_[t]);
            camera_.transform_.position = 
                target_.GetPosition() - 
                camera_.transform_.GetForwardVector() * 35.0f;

            found = true;
            lastFindIndex_ = t;
            break;
        }
    }

    if (!found)
        notificaiton_.Set(entityName + " not found");

    return CB_Stay;
}
