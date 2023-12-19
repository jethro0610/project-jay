#pragma once
#include "EditorMode.h"

class PlanarScaleMode : public EditorMode {
public:
    PlanarScaleMode(EditorModeArgs args); 
    std::string GetName() override { return "Planar Scale"; }
    int GetBinding() override { return 'Z'; }
    void Update() override;
};
