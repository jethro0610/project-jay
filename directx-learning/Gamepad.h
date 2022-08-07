#pragma once
#include <bitset>

#define GAMEPAD_A 0
#define GAMEPAD_B 1
#define GAMEPAD_X 2
#define GAMEPAD_Y 3
#define GAMEPAD_UP 4
#define GAMEPAD_DOWN 5
#define GAMEPAD_LEFT 6
#define GAMEPAD_RIGHT 7
#define GAMEPAD_LSHOULDER 8
#define GAMEPAD_RSHOULDER 9
#define GAMEPAD_RTRIGGER 10
#define GAMEPAD_LTRIGGER 11
#define GAMEPAD_LTHUMB 12
#define GAMEPAD_RTHUMB 13
#define GAMEPAD_START 14
#define GAMEPAD_SELECT 15

class Gamepad {
public:
    Gamepad();
    ~Gamepad();

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

