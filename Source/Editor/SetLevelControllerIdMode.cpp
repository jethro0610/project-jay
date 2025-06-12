#include "SetLevelControllerIdMode.h"
#include "Terrain/Terrain.h"
#include "Level/Level.h"
#include "EditorNotification.h"
#include "EditorTextInput.h"

SetLevelControllerIdMode::SetLevelControllerIdMode(EditorModeArgs args):
EditorMode(args) {
    mouseVisibile_ = false;
    requiresTarget_ = false;
    requiresLevel_ = true;
    ctrl_ = false;
}

void SetLevelControllerIdMode::OnStart() {
    textInput_.SetLabel("ID: ");
    textInput_.SetInput(std::to_string(level_.controllerId_));
    EditorMode::OnStart();
}

ConfirmBehavior SetLevelControllerIdMode::OnConfirm() {
    int controllerId;
    try {
        controllerId = std::stoi(textInput_.Get());
    }
    catch(const std::runtime_error& e) {
        return CB_Stay;
    }

    if (!level_.IsValidControllerId(controllerId)) {
        notificaiton_.Set("Invalid controller id");
        return CB_Stay;
    }

    level_.controllerId_ = controllerId;
    return CB_Default;
}


void SetLevelControllerIdMode::OnEnd() {
    textInput_.Clear();
    EditorMode::OnEnd();
}

void SetLevelControllerIdMode::Update() {
    textInput_.ReadInput();
}
