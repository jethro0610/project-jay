#pragma once
#include "EditorMode.h"

class CameraMode : public EditorMode {
public:
    CameraMode(EditorModeArgs args);
    void Update() override;
    std::string GetName() override { return "Camera"; }
    int GetBinding() override { return 'Q'; }
};
