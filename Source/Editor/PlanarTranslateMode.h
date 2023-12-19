#pragma once
#include "EditorMode.h"

class PlanarTranslateMode : public EditorMode {
public:
    PlanarTranslateMode(EditorModeArgs args); 
    std::string GetName() override { return "Planar Translation"; }
    int GetBinding() override { return 'E'; }
    void Update() override;
};
