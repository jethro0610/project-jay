#pragma once
#include "EditorMode.h"

class UniformScaleMode : public EditorMode {
public:
    UniformScaleMode(EditorModeArgs args); 
    std::string GetName() override { return "Uniform Scale"; }
    int GetBinding() override { return 'C'; }
    void Update() override;
};
