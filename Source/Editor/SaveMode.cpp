#include "SaveMode.h"
#include "Level/Level.h"
#include "EditorNotification.h"
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
    textInput_.SetInput(level_.DBG_name_.substr(3, level_.DBG_name_.length())); 
    EditorMode::OnStart();
}

ConfirmBehavior SaveMode::OnConfirm() {
    notificaiton_.Set("Saving lv_" + textInput_.Get() + "...");
    return CB_PostConfirm;
}

ConfirmBehavior SaveMode::PostConfirm() {
    level_.Save("lv_" + textInput_.Get());
    notificaiton_.Set("Saved lv_" + textInput_.Get());
    return CB_Default;
}

void SaveMode::OnEnd() {
    textInput_.Clear();
    EditorMode::OnConfirm();
}

void SaveMode::Update() {
    textInput_.ReadInput();
}
