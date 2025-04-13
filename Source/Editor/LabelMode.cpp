#include "LabelMode.h"
#include "EditorNotification.h"
#include "EditorTextInput.h"
#include "EditorTargetController.h"

LabelMode::LabelMode(EditorModeArgs args):
EditorMode(args) {
    requiresTarget_ = true;
    requiresLevel_ = true;
    mouseVisibile_ = false;
}

void LabelMode::OnStart() {
    textInput_.SetLabel("Set Label: "); 
    textInput_.SetInput(target_.Get().GetLabel()); 
    EditorMode::OnStart();
}

void LabelMode::OnEnd() {
    textInput_.Clear();
}

void LabelMode::Update() {
    textInput_.ReadInput();
}

ConfirmBehavior LabelMode::OnConfirm() {
    target_.Get().SetLabel(textInput_.Get());
    return CB_Default;
}
