#pragma once
#include "EditorMode.h"
#include <glm/vec4.hpp>

enum TranslateSubmode {
    TS_Planar,
    TS_Terrain,
    TS_Vertical,
};

class TranslateMode : public EditorMode {
public:
    TranslateMode(EditorModeArgs args); 
    int GetBinding() override { return 'T'; }
    std::string GetName() override;
    void OnStart() override;
    void OnCancel() override;
    void Update() override;

private:
    void SetSubmode(TranslateSubmode submode);
    TranslateSubmode submode_;
    glm::vec3 startPosition_;
    float deltaX_;
    float deltaY_;
};
