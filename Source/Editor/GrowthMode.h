#pragma once
#include "EditorMode.h"
#include "Spread/SpreadType.h"
#include <glm/vec3.hpp>

class GrowthMode : public EditorMode {
public:
    GrowthMode(EditorModeArgs args); 
    int GetBinding() override { return 'G'; }
    std::string GetName() override;
    void Update() override;
    void OnStart() override;
    void OnEnd() override;
    void SetType(SpreadType type);

    SpreadType type_;
};
