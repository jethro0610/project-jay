#pragma once
#include "EditorMode.h"

class VerticalTranslateMode : public EditorMode {
public:
    VerticalTranslateMode(EditorModeArgs args); 
    std::string GetName() override { return "Vertical Translation"; }
    int GetBinding() override { return 'R'; }
    void Update() override;
};
