#pragma once
#include <bitset>
#include <stdint.h>

const uint8_t GAMEPAD_A = 0;
const uint8_t GAMEPAD_B = 1;
const uint8_t GAMEPAD_X = 2;
const uint8_t GAMEPAD_Y = 3;
const uint8_t GAMEPAD_UP = 11;
const uint8_t GAMEPAD_DOWN = 13;
const uint8_t GAMEPAD_LEFT = 14;
const uint8_t GAMEPAD_RIGHT = 12;
const uint8_t GAMEPAD_LSHOULDER = 4;
const uint8_t GAMEPAD_RSHOULDER = 5;
const uint8_t GAMEPAD_RTRIGGER = 15;
const uint8_t GAMEPAD_LTRIGGER = 16;
const uint8_t GAMEPAD_LTHUMB = 9;
const uint8_t GAMEPAD_RTHUMB = 10;
const uint8_t GAMEPAD_START = 7;
const uint8_t GAMEPAD_SELECT = 6;
const uint8_t GAMEPAD_BUTTONS = 17;
const float TRIGGER_LIMIT = 0.5f;

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

    void SetButtonHeld(uint8_t btn, bool held);
};

