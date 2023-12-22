#include "NewLevelMode.h"
#include "EditorTextInput.h"

NewLevelMode::NewLevelMode(EditorModeArgs args):
EditorMode(args) {
    mouseVisibile_ = false;
    requiresTarget_ = false;
}

void NewLevelMode::OnStart() {
    textInput_.SetLabel("Name: lv_");
}

bool NewLevelMode::OnConfirm() {
    textInput_.ClearInput();
    return true;
}

void NewLevelMode::OnEnd() {
    textInput_.Clear();
}

void NewLevelMode::Update() {
    textInput_.ReadInput();
}
