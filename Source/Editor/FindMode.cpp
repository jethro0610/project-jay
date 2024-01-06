#include "FindMode.h"
#include "Camera/Camera.h"
#include "Entity/EntityListS.h"
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
    std::string entityName = "e_" + textInput_.Get();

    bool found = false;
    for (int i = 0; i < 128; i++) {
        int e = (i + lastFind_ + 1) % 128;
        if (!entities_.Valid(e)) continue;
        
        if (entities_.GetName(e) == entityName) {
            lastFind_ = e;
            target_.Set(e);
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
