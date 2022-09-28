#pragma once
#include <bitset>

const uint8_t GAMEPAD_A = 0;
const uint8_t GAMEPAD_B = 1;
const uint8_t GAMEPAD_X = 2;
const uint8_t GAMEPAD_Y = 3;
const uint8_t GAMEPAD_UP = 4;
const uint8_t GAMEPAD_DOWN = 5;
const uint8_t GAMEPAD_LEFT = 6;
const uint8_t GAMEPAD_RIGHT = 7;
const uint8_t GAMEPAD_LSHOULDER = 8;
const uint8_t GAMEPAD_RSHOULDER = 9;
const uint8_t GAMEPAD_RTRIGGER = 10;
const uint8_t GAMEPAD_LTRIGGER = 11;
const uint8_t GAMEPAD_LTHUMB = 12;
const uint8_t GAMEPAD_RTHUMB = 13;
const uint8_t GAMEPAD_START = 14;
const uint8_t GAMEPAD_SELECT = 15;

class Gamepad {
public:
    Gamepad();

    float leftStickX_;
    float leftStickY_;

    float rightStickX_;
    float rightStickY_;

    float leftTrigger_;
    float rightTrigger_;
    float triggerLimit_;

    std::bitset<16> heldButtons_;
    std::bitset<16> pressedButtons_;
    std::bitset<16> releasedButtons_;

    void SetButtonHeld(uint8_t btn, bool held);
};

