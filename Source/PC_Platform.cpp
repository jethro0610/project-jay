#include <assert.h>
#include <iostream>
#include "PC_Platform.h"

Platform* Platform::platform_ = nullptr;

void KeyCallback(GLFWwindow* window, int key, int scancode, int aciton, int mods);
void CursorCallback(GLFWwindow* window, double x, double y);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

Platform::Platform() {
    assert(platform_ == nullptr);

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window_ = glfwCreateWindow(1280, 720, "Project Jay", NULL, NULL);

    glfwSetKeyCallback(window_, KeyCallback); 

    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window_, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwGetCursorPos(window_, &mouseX_, &mouseY_);
    glfwSetCursorPosCallback(window_, CursorCallback);
    glfwSetMouseButtonCallback(window_, MouseButtonCallback);

    platform_ = this;
}

Platform* Platform::Get() {
    assert(platform_ != nullptr);
    return platform_;
}

GLFWwindow* Platform::GetWindow() {
    assert(platform_ != nullptr);
    return platform_->window_;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key < 0)
        return;

    switch (action) {
        case GLFW_PRESS:
            Platform::Get()->OnKeyDown(key);
            break;

        case GLFW_RELEASE:
            Platform::Get()->OnKeyUp(key);
            break;
    }
}

void Platform::OnKeyDown(uint16_t key) {
    heldKeys_[key] = true;
    pressedKeys_[key] = true;
}

void Platform::OnKeyUp(uint16_t key) {
    heldKeys_[key] = false;
    releasedKeys_[key] = true;
}

void Platform::FlushKeys() {
    pressedKeys_.reset();
    releasedKeys_.reset();
}

void CursorCallback(GLFWwindow* window, double x, double y) {
    Platform* platform = Platform::Get();
    platform->mouseX_ = x;
    platform->mouseY_ = y;
}

void Platform::UpdateMouseMovement() {
    deltaMouseX_ = mouseX_ - lastMouseX_;   
    deltaMouseY_ = mouseY_ - lastMouseY_;
}

void Platform::FlushMouse() {
    deltaMouseX_ = 0;
    deltaMouseY_ = 0;

    lastMouseX_ = mouseX_;
    lastMouseY_ = mouseY_;
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button > 1)
        return;

    switch (action) {
        case GLFW_PRESS:
            Platform::Get()->OnKeyDown(MOUSE_KEYS[button]);
            break;

        case GLFW_RELEASE:
            Platform::Get()->OnKeyUp(MOUSE_KEYS[button]);
            break;
    }
}
