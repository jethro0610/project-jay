#pragma once
#include <bitset>

class Gamepad {
public:
    static constexpr int A = 0;
    static constexpr int B = 1;
    static constexpr int X = 2;
    static constexpr int Y = 3;
    static constexpr int UP = 11;
    static constexpr int DOWN = 13;
    static constexpr int LEFT = 14;
    static constexpr int RIGHT = 12;
    static constexpr int LSHOULDER = 4;
    static constexpr int RSHOULDER = 5;
    static constexpr int RTRIGGER = 15;
    static constexpr int LTRIGGER = 16;
    static constexpr int LTHUMB = 9;
    static constexpr int RTHUMB = 10;
    static constexpr int START = 7;
    static constexpr int SELECT = 6;
    static constexpr int BUTTONS = 17;
    static constexpr float TRIGGER_LIMIT = 0.5f;
    Gamepad();

    float leftStickX_;
    float leftStickY_;

    float rightStickX_;
    float rightStickY_;

    float leftTrigger_;
    float rightTrigger_;

    std::bitset<BUTTONS> heldButtons_;
    std::bitset<BUTTONS> pressedButtons_;
    std::bitset<BUTTONS> releasedButtons_;

    void SetButtonHeld(int btn, bool held);
};

