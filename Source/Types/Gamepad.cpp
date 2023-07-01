#include "Gamepad.h"

Gamepad::Gamepad() {
    float leftStickX_ = 0.0f;
    float leftStickY_ = 0.0f;

    float rightStickX_ = 0.0f;
    float rightStickY_ = 0.0f;

    float leftTrigger_ = 0.0f;
    float rightTrigger_ = 0.0f;

    heldButtons_ = 0;
    pressedButtons_ = 0;
    releasedButtons_ = 0;
}

void Gamepad::SetButtonHeld(int btn, bool held) {
    if (heldButtons_[btn] == false && held == true) {
        pressedButtons_[btn] = true;
    }
    else if (heldButtons_[btn] == true && held == false) {
        releasedButtons_[btn] = true;
    }
    heldButtons_[btn] = held;
}
