#pragma once
#include "EditorMode.h"
#include <glm/vec3.hpp>

class WeedMode : public EditorMode {
public:
    WeedMode(EditorModeArgs args); 
    int GetBinding() override { return 'G'; }
    std::string GetName() override;
    void Update() override;
};
