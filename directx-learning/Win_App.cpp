#include "App.h"
#include <Xinput.h>
#include <sstream>

App::App() {
    running_ = false;
    windowsLayer_ = nullptr;
    forwardInput_ = 0;
    sideInput_ = 0;
}

void App::Run() {
    running_ = true;
    windowsLayer_ = WindowsLayer::InitWindowsLayer(800, 600, "DirectXLearning");
    MSG msg;
    while (!windowsLayer_->closed_ && running_) {
        windowsLayer_->ClearPressedAndReleasedKeys();
        windowsLayer_->ClearMouseMovement();

        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        windowsLayer_->UpdateMouseMovement();
        PollGamepadInputs();
        UpdateInputs();
    }
}

void App::PollGamepadInputs() {
    XINPUT_STATE state;

    gamepad.pressedButtons_.reset();
    gamepad.releasedButtons_.reset();
    gamepad.leftStickX_ = 0.0f;
    gamepad.leftStickY_ = 0.0f;
    gamepad.rightStickX_ = 0.0f;
    gamepad.rightStickY_ = 0.0f;
    gamepad.leftTrigger_ = 0.0f;
    gamepad.rightTrigger_ = 0.0f;
    for (DWORD controllerIndex = 0; controllerIndex < XUSER_MAX_COUNT; controllerIndex++) {
        if (XInputGetState(0, &state) != ERROR_SUCCESS) continue;
        XINPUT_GAMEPAD xGamepad = state.Gamepad;

        gamepad.leftStickX_ = (float)xGamepad.sThumbLX / SHRT_MAX;
        gamepad.leftStickY_ = (float)xGamepad.sThumbLY / SHRT_MAX;
        gamepad.rightStickX_ = (float)xGamepad.sThumbRX / SHRT_MAX;
        gamepad.rightStickY_ = (float)xGamepad.sThumbRY / SHRT_MAX;

        gamepad.SetButtonHeld(GAMEPAD_A, xGamepad.wButtons & XINPUT_GAMEPAD_A);
        gamepad.SetButtonHeld(GAMEPAD_B, xGamepad.wButtons & XINPUT_GAMEPAD_B);
        gamepad.SetButtonHeld(GAMEPAD_X, xGamepad.wButtons & XINPUT_GAMEPAD_X);
        gamepad.SetButtonHeld(GAMEPAD_Y, xGamepad.wButtons & XINPUT_GAMEPAD_Y);
        gamepad.SetButtonHeld(GAMEPAD_UP, xGamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
        gamepad.SetButtonHeld(GAMEPAD_DOWN, xGamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
        gamepad.SetButtonHeld(GAMEPAD_LEFT, xGamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
        gamepad.SetButtonHeld(GAMEPAD_RIGHT, xGamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
        gamepad.SetButtonHeld(GAMEPAD_LSHOULDER, xGamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
        gamepad.SetButtonHeld(GAMEPAD_RSHOULDER, xGamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
        gamepad.SetButtonHeld(GAMEPAD_LTHUMB, xGamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
        gamepad.SetButtonHeld(GAMEPAD_RTHUMB, xGamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
        gamepad.SetButtonHeld(GAMEPAD_START, xGamepad.wButtons & XINPUT_GAMEPAD_START);
        gamepad.SetButtonHeld(GAMEPAD_SELECT, xGamepad.wButtons & XINPUT_GAMEPAD_BACK);

        gamepad.leftTrigger_ = (float)xGamepad.bLeftTrigger / MAXBYTE;
        gamepad.rightTrigger_ = (float)xGamepad.bRightTrigger / MAXBYTE;

        gamepad.SetButtonHeld(GAMEPAD_LTRIGGER, gamepad.leftTrigger_ > gamepad.triggerLimit_);
        gamepad.SetButtonHeld(GAMEPAD_RTRIGGER, gamepad.rightTrigger_ > gamepad.triggerLimit_);
    }
}

void App::UpdateInputs() {
    forwardInput_ = 0.0f;
    sideInput_ = 0.0f;

    if (windowsLayer_->heldKeys_['W'])
        forwardInput_ += 1.0f;

    if (windowsLayer_->heldKeys_['S'])
        forwardInput_ -= 1.0f;

    if (windowsLayer_->heldKeys_['D'])
        sideInput_ += 1.0f;

    if (windowsLayer_->heldKeys_['A'])
        sideInput_ -= 1.0f;

    forwardInput_ += gamepad.leftStickY_;
    sideInput_ += gamepad.leftStickX_;

    std::stringstream ss;
    ss << "Forward: " << forwardInput_ << " Side: " << sideInput_;
    SetWindowText(windowsLayer_->windowHandle_, ss.str().c_str());
}