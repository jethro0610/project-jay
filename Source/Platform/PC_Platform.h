#pragma once
#include "Types/Gamepad.h"
#include <bitset>
class GLFWwindow;

const int NUM_KEYS = 349;
const int LEFT_MOUSE_KEY = NUM_KEYS;
const int RIGHT_MOUSE_KEY = NUM_KEYS + 1;
const int MOUSE_KEYS[] = { LEFT_MOUSE_KEY, RIGHT_MOUSE_KEY };

class Platform {
public:
    Platform();
    GLFWwindow* window_;

    std::bitset<NUM_KEYS + 2> heldKeys_;
    std::bitset<NUM_KEYS + 2> pressedKeys_;
    std::bitset<NUM_KEYS + 2> releasedKeys_;

    Gamepad gamepad_;
    double mouseX_;
    double mouseY_;
    double lastMouseX_;
    double lastMouseY_;
    double deltaMouseX_;
    double deltaMouseY_;
    bool changedVisibility_;

    static Platform* Get();
    static GLFWwindow* GetWindow();

    void OnKeyDown(int key);
    void OnKeyUp(int key);
    void FlushKeys();

    void UpdateMouseMovement();
    void FlushMouse();

    void PollGamepad();
    void SetMouseVisible(bool visible);

private:
    static Platform* platform_;
};
