#pragma once
#include <bitset>
#include "../Types/Gamepad.h"
class GLFWwindow;

const uint16_t NUM_KEYS = 349;
const uint16_t LEFT_MOUSE_KEY = NUM_KEYS;
const uint16_t RIGHT_MOUSE_KEY = NUM_KEYS + 1;
const uint16_t MOUSE_KEYS[] = { LEFT_MOUSE_KEY, RIGHT_MOUSE_KEY };

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

    static Platform* Get();
    static GLFWwindow* GetWindow();

    void OnKeyDown(uint16_t key);
    void OnKeyUp(uint16_t key);
    void FlushKeys();

    void UpdateMouseMovement();
    void FlushMouse();

    void PollGamepad();

private:
    static Platform* platform_;
};
