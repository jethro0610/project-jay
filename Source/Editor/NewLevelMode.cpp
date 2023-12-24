#include "NewLevelMode.h"
#include "EditorLevel.h"
#include "EditorNotification.h"
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
    EditorMode::OnStart();
}

ConfirmBehavior NewLevelMode::OnConfirm() {
    notificaiton_.Set("Creating lv_" + textInput_.Get() + "...");
    return CB_PostConfirm;
}

ConfirmBehavior NewLevelMode::PostConfirm() {
    level_.New("lv_" + textInput_.Get());
    notificaiton_.Set("Created lv_" + textInput_.Get());
    return CB_Default;
}

void NewLevelMode::OnEnd() {
    textInput_.Clear();
    EditorMode::OnEnd();
}

void NewLevelMode::Update() {
    textInput_.ReadInput();
}
