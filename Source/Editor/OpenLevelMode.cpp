#include "OpenLevelMode.h"
#include "EditorLevel.h"
#include "EditorTextInput.h"

OpenLevelMode::OpenLevelMode(EditorModeArgs args):
EditorMode(args) {
    mouseVisibile_ = false;
    requiresTarget_ = false;
    requiresLevel_ = false;
    ctrl_ = true;
}

void OpenLevelMode::OnStart() {
    textInput_.SetLabel("lv_");
    EditorMode::OnStart();
}

bool OpenLevelMode::OnConfirm() {
    return level_.Load(textInput_.Get());
}

void OpenLevelMode::OnEnd() {
    textInput_.Clear();
    EditorMode::OnConfirm();
}

void OpenLevelMode::Update() {
    textInput_.ReadInput();
}
