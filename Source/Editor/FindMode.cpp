#include "FindMode.h"
#include "Camera/Camera.h"
#include "Entity/EntityList.h"
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
    for (int i = 0; i < 128; i++) {
        int e = (i + lastFindIndex_ + 1) % 128;
        if (!entities_[e].alive_) continue;
        
        if (entities_[e].DBG_name_ == entityName) {
            lastFindIndex_ = e;
            target_.Set(&entities_[e]);
            camera_.transform_.position = 
                entities_[e].transform_.position - 
                camera_.transform_.GetForwardVector() * 35.0f;
            found = true;
            break;
        }
    }

    if (!found)
        notificaiton_.Set(entityName + " not found");

    return CB_Stay;
}
