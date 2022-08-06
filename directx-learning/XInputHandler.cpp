#include "XInputHandler.h"

XInputHandler::XInputHandler() {
    float leftStickX_ = 0.0f;
    float leftStickY_ = 0.0f;

    float rightStickX_ = 0.0f;
    float rightStickY_ = 0.0f;

    float leftTrigger_ = 0.0f;
    float rightTrigger_ = 0.0f;
    float triggerLimit_ = 0.0f;
}

void XInputHandler::Poll() {
    XINPUT_STATE state;
    
    pressedButtons_.reset();
    releasedButtons_.reset();
    leftStickX_ = 0.0f;
    leftStickY_ = 0.0f;
    rightStickX_ = 0.0f;
    rightStickY_ = 0.0f;
    leftTrigger_ = 0.0f;
    rightTrigger_ = 0.0f;
    for (DWORD controllerIndex = 0; controllerIndex < XUSER_MAX_COUNT; controllerIndex++) {
        if (XInputGetState(0, &state) != ERROR_SUCCESS) continue;
        XINPUT_GAMEPAD gamepad = state.Gamepad;

        leftStickX_ = (float)gamepad.sThumbLX / SHRT_MAX;
        leftStickY_ = (float)gamepad.sThumbLY / SHRT_MAX;
        rightStickX_ = (float)gamepad.sThumbRX / SHRT_MAX;
        rightStickY_ = (float)gamepad.sThumbRY / SHRT_MAX;

        SetButtonHeld(GAMEPAD_A, gamepad.wButtons & XINPUT_GAMEPAD_A);
        SetButtonHeld(GAMEPAD_B, gamepad.wButtons & XINPUT_GAMEPAD_B);
        SetButtonHeld(GAMEPAD_X, gamepad.wButtons & XINPUT_GAMEPAD_X);
        SetButtonHeld(GAMEPAD_Y, gamepad.wButtons & XINPUT_GAMEPAD_Y);
        SetButtonHeld(GAMEPAD_UP, gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
        SetButtonHeld(GAMEPAD_DOWN, gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
        SetButtonHeld(GAMEPAD_LEFT, gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
        SetButtonHeld(GAMEPAD_RIGHT, gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
        SetButtonHeld(GAMEPAD_LSHOULDER, gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
        SetButtonHeld(GAMEPAD_RSHOULDER, gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
        SetButtonHeld(GAMEPAD_LTHUMB, gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
        SetButtonHeld(GAMEPAD_RTHUMB, gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
        SetButtonHeld(GAMEPAD_START, gamepad.wButtons & XINPUT_GAMEPAD_START);
        SetButtonHeld(GAMEPAD_SELECT, gamepad.wButtons & XINPUT_GAMEPAD_BACK);

        leftTrigger_ = (float)gamepad.bLeftTrigger / MAXBYTE;
        rightTrigger_ = (float)gamepad.bRightTrigger / MAXBYTE;

        SetButtonHeld(GAMEPAD_LTRIGGER, leftTrigger_ > triggerLimit_);
        SetButtonHeld(GAMEPAD_RTRIGGER, rightTrigger_ > triggerLimit_);
    }
}

void XInputHandler::SetButtonHeld(uint8_t btn, bool held) {
    if (heldButtons_[btn] == false && held == true) {
        pressedButtons_[btn] = true;
    }
    else if (heldButtons_[btn] == true && held == false) {
        releasedButtons_[btn] = true;
    }
    heldButtons_[btn] = held;
}