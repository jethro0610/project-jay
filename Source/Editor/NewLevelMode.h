#pragma once
#include "EditorMode.h"

class NewLevelMode : public EditorMode {
public:
    NewLevelMode(EditorModeArgs args); 
    int GetBinding() override { return 'N'; }
    std::string GetName() override { return "New Level"; };
    void OnStart() override;
    ConfirmBehavior OnConfirm() override;
    ConfirmBehavior PostConfirm() override;
    void OnEnd() override;
    void Update() override;
};
