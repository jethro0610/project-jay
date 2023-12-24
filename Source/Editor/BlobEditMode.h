#pragma once
#include "EditorMode.h"
#include <glm/vec3.hpp>

enum BlobEditPhase {
    BE_SelectProperty,
    BE_Seed,
    BE_Frequency,
    BE_MinRadius,
    BE_MaxRadius
};

class BlobEditMode : public EditorMode {
public:
    BlobEditMode(EditorModeArgs args); 
    int GetBinding() override { return 'N'; }
    std::string GetName() override { return "Terrain Edit"; };
    void OnStart() override;
    void OnEnd() override;
    ConfirmBehavior OnConfirm() override;
    ConfirmBehavior PostConfirm() override;
    void Update() override;

private:
    void SetPhase(BlobEditPhase phase); 
    BlobEditPhase phase_;
};
