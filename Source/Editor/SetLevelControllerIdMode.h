#pragma once
#include "EditorMode.h"

class SetLevelControllerIdMode : public EditorMode {
public:
    SetLevelControllerIdMode(EditorModeArgs args); 
    int GetBinding() override { return 'O'; }
    std::string GetName() override { return "Set Level Controller Id"; };
    void OnStart() override;
    ConfirmBehavior OnConfirm() override;
    void OnEnd() override;
    void Update() override;
};
