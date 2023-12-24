#pragma once
#include "EditorMode.h"

class SaveMode : public EditorMode {
public:
    SaveMode(EditorModeArgs args); 
    int GetBinding() override { return 'S'; }
    std::string GetName() override { return "Save Level"; };
    void OnStart() override;
    bool OnConfirm() override;
    void OnEnd() override;
    void Update() override;
};
