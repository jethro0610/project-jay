#pragma once
#include "EditorMode.h"

class VerticalScaleMode : public EditorMode {
public:
    VerticalScaleMode(EditorModeArgs args); 
    std::string GetName() override { return "Vertical Scale"; }
    int GetBinding() override { return 'X'; }
    void Update() override;
};
