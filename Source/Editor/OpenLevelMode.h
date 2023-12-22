#pragma once
#include "EditorMode.h"

class OpenLevelMode : public EditorMode {
public:
    OpenLevelMode(EditorModeArgs args); 
    int GetBinding() override { return 'O'; }
    std::string GetName() override { return "Open Level"; };
    void OnStart() override;
    bool OnConfirm() override;
    void OnEnd() override;
    void Update() override;
};
