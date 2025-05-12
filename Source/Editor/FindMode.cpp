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
}

void FindMode::OnStart() {
    lastFindIndex_ = 0;
    textInput_.SetLabel("Find Target: ");
    EditorMode::OnStart();
}

void FindMode::OnEnd() {
    textInput_.Clear();
}

void FindMode::Update() {
    textInput_.ReadInput();
}

ConfirmBehavior FindMode::OnConfirm() {
    bool found = false;
    for (int i = 0; i < targets_.size(); i++) {
        int t = (i + lastFindIndex_ + 1) % targets_.size();
        if (!targets_[t]->Selectable()) continue;
        DEBUGLOG(targets_[t]->GetName());
        
        if (targets_[t]->GetName() == textInput_.Get() || targets_[t]->GetLabel() == textInput_.Get()) {
            target_.Select(targets_[t]);
            camera_.transform_.position = 
                target_.Get().GetPosition() - 
                camera_.transform_.GetForwardVector() * 100.0f;

            found = true;
            lastFindIndex_ = t;
            break;
        }
    }

    if (!found)
        notificaiton_.Set(textInput_.Get() + " not found");

    return CB_Stay;
}
