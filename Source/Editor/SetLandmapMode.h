#pragma once
#include "EditorMode.h"

class SetLandmapMode : public EditorMode {
public:
    SetLandmapMode(EditorModeArgs args); 
    int GetBinding() override { return 'L'; }
    std::string GetName() override { return "Set Landmap"; };
    void OnStart() override;
    ConfirmBehavior OnConfirm() override;
    ConfirmBehavior PostConfirm() override;
    void OnEnd() override;
    void Update() override;
};
