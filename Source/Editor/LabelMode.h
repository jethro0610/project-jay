#pragma once
#include "EditorMode.h"
#include "Spread/SpreadType.h"

class LabelMode : public EditorMode {
public:
    LabelMode(EditorModeArgs args); 
    int GetBinding() override { return 'N'; }
    std::string GetName() override { return "Label"; }
    void OnStart() override;
    void OnEnd() override;
    ConfirmBehavior OnConfirm() override;
    void Update() override;

    SpreadType type_;
};
