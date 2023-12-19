#pragma once
#include "EditorMode.h"

class TerrainTranslateMode : public EditorMode {
public:
    TerrainTranslateMode(EditorModeArgs args); 
    std::string GetName() override { return "Terrain Aligned Translation"; }
    int GetBinding() override { return 'T'; }
    void Update() override;
};
