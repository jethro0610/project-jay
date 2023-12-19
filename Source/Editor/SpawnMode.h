#pragma once
#include "EditorMode.h"

class SpawnMode : public EditorMode {
public:
    SpawnMode(EditorModeArgs args); 
    std::string GetName() override { return "Spawn Entity"; }
    int GetBinding() override { return 'I'; }
    void OnStart() override;
    void OnEnd() override;
    void Update() override;
};
