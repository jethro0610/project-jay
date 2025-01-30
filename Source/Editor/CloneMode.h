#pragma once
#include "EditorMode.h"
#include "Types/Transform.h"
#include <glm/vec3.hpp>
#include <GLFW/glfw3.h>

class Entity;

class CloneMode : public EditorMode {
    enum CloneSubmode {
        CS_Planar,
        CS_Terrain
    };
public:
    CloneMode(EditorModeArgs args);
    std::string GetName() override;
    int GetBinding() override { return 'D'; }
    void OnStart() override;
    void OnEnd() override;
    ConfirmBehavior OnConfirm() override;
    void Update() override;

private:
    void SetSubmode(CloneSubmode);
    CloneSubmode submode_;

    float deltaX_;
    float deltaY_;

    EditorTarget* original_;
};
