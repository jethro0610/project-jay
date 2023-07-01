#pragma once
#include <bitset>
#include <stdint.h>

const int GAMEPAD_A = 0;
const int GAMEPAD_B = 1;
const int GAMEPAD_X = 2;
const int GAMEPAD_Y = 3;
const int GAMEPAD_UP = 11;
const int GAMEPAD_DOWN = 13;
const int GAMEPAD_LEFT = 14;
const int GAMEPAD_RIGHT = 12;
const int GAMEPAD_LSHOULDER = 4;
const int GAMEPAD_RSHOULDER = 5;
const int GAMEPAD_RTRIGGER = 15;
const int GAMEPAD_LTRIGGER = 16;
const int GAMEPAD_LTHUMB = 9;
const int GAMEPAD_RTHUMB = 10;
const int GAMEPAD_START = 7;
const int GAMEPAD_SELECT = 6;
const int GAMEPAD_BUTTONS = 17;
const int TRIGGER_LIMIT = 0.5f;

class Gamepad {
public:
    Gamepad();

    float leftStickX_;
    float leftStickY_;

    float rightStickX_;
    float rightStickY_;

    float leftTrigger_;
    float rightTrigger_;

    std::bitset<GAMEPAD_BUTTONS> heldButtons_;
    std::bitset<GAMEPAD_BUTTONS> pressedButtons_;
    std::bitset<GAMEPAD_BUTTONS> releasedButtons_;

    void SetButtonHeld(int btn, bool held);
};

