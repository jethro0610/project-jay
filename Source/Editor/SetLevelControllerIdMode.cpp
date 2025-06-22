#include "SetLevelControllerIdMode.h"
#include "Terrain/Terrain.h"
#include "Level/Level.h"
#include "LevelControllers/LevelController.h"
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
    textInput_.SetInput(std::to_string(levelController_.id_));
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

    if (!LevelController::IsValidControllerId(controllerId)) {
        notificaiton_.Set("Invalid controller id");
        return CB_Stay;
    }

    levelController_.id_ = controllerId;
    return CB_Default;
}


void SetLevelControllerIdMode::OnEnd() {
    textInput_.Clear();
    EditorMode::OnEnd();
}

void SetLevelControllerIdMode::Update() {
    textInput_.ReadInput();
}
