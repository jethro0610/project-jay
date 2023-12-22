#include "NewLevelMode.h"
#include "EditorLevel.h"
#include "EditorTextInput.h"

NewLevelMode::NewLevelMode(EditorModeArgs args):
EditorMode(args) {
    mouseVisibile_ = false;
    requiresTarget_ = false;
    requiresLevel_ = false;
    ctrl_ = true;
}

void NewLevelMode::OnStart() {
    textInput_.SetLabel("lv_");
}

bool NewLevelMode::OnConfirm() {
    level_.New("lv_" + textInput_.Get());
    textInput_.ClearInput();
    return true;
}

void NewLevelMode::OnEnd() {
    textInput_.Clear();
}

void NewLevelMode::Update() {
    textInput_.ReadInput();
}
