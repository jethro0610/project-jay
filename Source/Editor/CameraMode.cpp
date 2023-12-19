#include "CameraMode.h"
#include "Editor.h"

CameraMode::CameraMode(EditorModeArgs args):
EditorMode(args)
{ 
    mouseVisibile_ = false;
}

void CameraMode::Update() {
    camera_.Update(inputs_);
}
