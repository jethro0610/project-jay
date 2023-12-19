#pragma once
#include "EditorMode.h"
#include <glm/vec3.hpp>
#include <GLFW/glfw3.h>

enum DefaultSubmode {
    DS_Camera,
    DS_Cursor
};

class DefaultMode : public EditorMode {
public:
    DefaultMode(EditorModeArgs args);
    void SetSubmode(DefaultSubmode submode);
    std::string GetName() override;
    int GetBinding() override { return GLFW_KEY_TAB; }
    void OnStart() override;
    void Update() override;
    glm::vec3 GetMouseRay();

private:
    DefaultSubmode submode_;
    void CameraUpdate();
    void CursorUpdate();
};
