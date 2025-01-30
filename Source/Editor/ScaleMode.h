#pragma once
#include "EditorMode.h"
#include <glm/vec4.hpp>

enum ScaleSubmode {
    SS_Uniform,
    SS_Vertical,
    SS_Planar,

    SS_X,
    SS_Y,
    SS_Z,
    SS_W
};

class ScaleMode : public EditorMode {
public:
    ScaleMode(EditorModeArgs args); 
    int GetBinding() override { return 'E'; }
    std::string GetName() override;
    void OnStart() override;
    void OnCancel() override;
    void Update() override;

private:
    void SetSubmode(ScaleSubmode submode);
    float fromZero_;
    glm::vec4 startScale_;
    ScaleSubmode submode_;
    float deltaX_;
    float deltaY_;
};
