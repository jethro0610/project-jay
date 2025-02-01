#include "GenericScalarMode.h"
#include "EditorTargetController.h"
#include "EditorTextInput.h"
#include "Platform/PC_Platform.h"
using namespace glm;

GenericScalarMode::GenericScalarMode(EditorModeArgs args):
EditorMode(args) 
{
    mouseVisibile_ = false;
    requiresTarget_ = true;
}

std::string GenericScalarMode::GetName() { 
    return "Set " + scalarNames_[curScalarId_];
}

void GenericScalarMode::OnStart() {
    scalarNames_ = target_.GetScalarNames();
    delta_ = 0.0f;
    originalValue_ = target_.GetScalar(curScalarId_);
    textInput_.SetLabel(scalarNames_[curScalarId_] + ": ");

    EditorMode::OnStart();
}

void GenericScalarMode::OnCancel() {
    target_.SetScalar(curScalarId_, originalValue_);
}

void GenericScalarMode::OnEnd() {
    textInput_.Clear();
    EditorMode::OnEnd();
}

void GenericScalarMode::Update() {
    textInput_.ReadInput();
    try {
        float val = std::stof(textInput_.Get());
        target_.SetScalar(curScalarId_, val);
    }
    catch (...) {
        float scalarDelta = target_.GetScalarDelta(curScalarId_);
        delta_ += platform_.deltaMouseX_ * 0.1f * scalarDelta;
        delta_ -= platform_.deltaMouseY_ * 0.1f * scalarDelta;
        target_.SetScalar(curScalarId_, originalValue_ + delta_);
    }
}
