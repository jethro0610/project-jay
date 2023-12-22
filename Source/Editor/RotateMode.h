#pragma once
#include "EditorMode.h"
#include <glm/gtx/quaternion.hpp>


class RotateMode : public EditorMode {
    enum RotateSubmode {
        RS_Free,
        RS_X,
        RS_Y,
        RS_Z,
        RS_Roll
    };

public:
    RotateMode(EditorModeArgs args); 
    int GetBinding() override { return 'R'; }
    std::string GetName() override;
    void OnStart() override;
    void OnCancel() override;
    void Update() override;

private:
    void SetSubmode(RotateSubmode submode);
    glm::quat startRotation_;
    bool fromZero_;
    RotateSubmode submode_;
    float deltaX_;
    float deltaY_;
};
