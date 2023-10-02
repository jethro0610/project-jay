#include "PC_Platform.h"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include <GLFW/glfw3.h>
#include "../Logging/Logger.h"
#include "../Helpers/Assert.h"

Platform* Platform::platform_ = nullptr;

void KeyCallback(GLFWwindow* window, int key, int scancode, int aciton, int mods);
void CursorCallback(GLFWwindow* window, double x, double y);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void LoadMappingFile();

Platform::Platform() {
    assert(platform_ == nullptr);
    DEBUGLOG("Starting PC platform w/ GLFW...");
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

    LoadMappingFile();

    DEBUGLOG("Succesfully started platform");
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

void Platform::OnKeyDown(int key) {
    heldKeys_[key] = true;
    pressedKeys_[key] = true;
}

void Platform::OnKeyUp(int key) {
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

void Platform::PollGamepad() {
    gamepad_.leftStickX_ = 0.0f;
    gamepad_.leftStickY_ = 0.0f;
    gamepad_.rightStickX_ = 0.0f;
    gamepad_.rightStickY_ = 0.0f;
    gamepad_.leftTrigger_ = 0.0f;
    gamepad_.rightTrigger_ = 0.0f;

    if (!glfwJoystickPresent(GLFW_JOYSTICK_1))
        return;

    GLFWgamepadstate state;
    if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state) == GLFW_FALSE)
        return;

    gamepad_.leftStickX_ = state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
    gamepad_.leftStickY_ = -state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
    gamepad_.rightStickX_ = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
    gamepad_.rightStickY_ = -state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];

    if (gamepad_.leftStickX_ < 0.1f && gamepad_.leftStickX_ > -0.1f)
        gamepad_.leftStickX_ = 0.0f;
    if (gamepad_.leftStickY_ < 0.1f && gamepad_.leftStickY_ > -0.1f)
        gamepad_.leftStickY_ = 0.0f;
    if (gamepad_.rightStickX_ < 0.1f && gamepad_.rightStickX_ > -0.1f)
        gamepad_.rightStickX_ = 0.0f;
    if (gamepad_.rightStickY_ < 0.1f && gamepad_.rightStickY_ > -0.1f)
        gamepad_.rightStickY_ = 0.0f;

    gamepad_.leftTrigger_ = (state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] + 1.0f) / 2.0f;
    gamepad_.rightTrigger_ = (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] + 1.0f) / 2.0f;
    gamepad_.SetButtonHeld(GAMEPAD_LTRIGGER, gamepad_.leftTrigger_ > TRIGGER_LIMIT);
    gamepad_.SetButtonHeld(GAMEPAD_RTRIGGER, gamepad_.rightTrigger_ > TRIGGER_LIMIT);

    for (int i = 0; i < GAMEPAD_BUTTONS - 2; i++)
        gamepad_.SetButtonHeld(i, state.buttons[i]);
}

void LoadMappingFile() {
    std::ifstream mappingFile("./mappings/gamecontrollerdb.txt");
    ASSERT(mappingFile.is_open(), "Failed to find input mapping file");

    std::string line, text;
    while (std::getline(mappingFile, line))
        text += line + '\n';

    const char* mappingData = text.c_str();
    ASSERT((glfwUpdateGamepadMappings(mappingData) == GLFW_TRUE), "Input mapping file invalid");
}
