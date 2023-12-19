#pragma once
#include "EditorMode.h"
#include <glm/vec3.hpp>
#include <GLFW/glfw3.h>

class CursorMode : public EditorMode {
public:
    CursorMode(EditorModeArgs args);
    std::string GetName() override { return "Cursor"; }
    int GetBinding() override { return GLFW_KEY_TAB; }
    void Update() override;
    glm::vec3 GetMouseRay();
};
