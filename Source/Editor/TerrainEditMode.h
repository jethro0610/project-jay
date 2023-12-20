#pragma once
#include "EditorMode.h"
#include <glm/vec3.hpp>

enum TerrainEditPhase {
    TE_SelectNoiseLayer,
    TE_SelectProperty,
    TE_Activate,
    TE_Deactivate,
    TE_Seed,
    TE_Frequency,
    TE_Multiplier,
    TE_Exponent
};

class TerrainEditMode : public EditorMode {
public:
    TerrainEditMode(EditorModeArgs args); 
    int GetBinding() override { return 'Y'; }
    std::string GetName() override { return "Terrain Edit"; };
    void OnStart() override;
    void OnEnd() override;
    bool OnConfirm() override;
    void Update() override;

private:
    void SetPhase(TerrainEditPhase phase); 
    TerrainEditPhase phase_;
    int targetLayer_;
    std::string LayerName() { return std::to_string(targetLayer_); } 
};
