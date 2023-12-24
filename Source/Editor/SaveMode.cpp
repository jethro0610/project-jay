#include "SaveMode.h"
#include "EditorLevel.h"
#include "EditorTextInput.h"

SaveMode::SaveMode(EditorModeArgs args):
EditorMode(args) {
    mouseVisibile_ = false;
    requiresTarget_ = false;
    requiresLevel_ = true;
    ctrl_ = true;
}

void SaveMode::OnStart() {
    textInput_.SetLabel("lv_");
    textInput_.SetInput(level_.name_.erase(0, 3)); 
    EditorMode::OnStart();
}

bool SaveMode::OnConfirm() {
    level_.Save("lv_" + textInput_.Get());
    return true;
}

void SaveMode::OnEnd() {
    textInput_.Clear();
    EditorMode::OnConfirm();
}

void SaveMode::Update() {
    textInput_.ReadInput();
}
