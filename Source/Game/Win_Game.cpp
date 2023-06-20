#include "Game.h"
#include "Time.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <sstream>
#include <iostream>

#ifdef _DEBUG
#include "../Logging/ScreenText.h"
#endif

void KeyCallback(GLFWwindow* window, int key, int scancode, int aciton, int mods);

Game::Game():
    timeAccumlulator_(0.0f),
    dxResources_(glfwGetWin32Window(platform_.window_), 1280, 720),
    resourceManager_(dxResources_),
    entityManager_(resourceManager_),
    renderer_(resourceManager_),
    camera_(entityManager_.GetComponent<TransformComponent>(), 14.0f),
    world_(resourceManager_),
    seedManager_(),
    spreadManager_(resourceManager_, seedManager_, world_),
    playerController_(),
    gamepad_(Gamepad()),
    running_(true)
{
    Init();

    while (running_) {
        Time::UpdateTime();

        platform_.FlushMouse();
        glfwPollEvents();
        // while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            // TranslateMessage(&msg);
            // DispatchMessage(&msg);
        // }
        platform_.UpdateMouseMovement();
        // windowsLayer_.UpdateMouseMovement();
        // PollGamepadInputs_P();
        UpdateInputs_P();
        Update();
    }
}

void Game::PollGamepadInputs_P() {
//     XINPUT_STATE state;
//
//     gamepad_.leftStickX_ = 0.0f;
//     gamepad_.leftStickY_ = 0.0f;
//     gamepad_.rightStickX_ = 0.0f;
//     gamepad_.rightStickY_ = 0.0f;
//     gamepad_.leftTrigger_ = 0.0f;
//     gamepad_.rightTrigger_ = 0.0f;
//     for (DWORD controllerIndex = 0; controllerIndex < XUSER_MAX_COUNT; controllerIndex++) {
//         if (XInputGetState(0, &state) != ERROR_SUCCESS) continue;
//         XINPUT_GAMEPAD xGamepad = state.Gamepad;
//
//         gamepad_.leftStickX_ = (float)xGamepad.sThumbLX / SHRT_MAX;
//         gamepad_.leftStickY_ = (float)xGamepad.sThumbLY / SHRT_MAX;
//         gamepad_.rightStickX_ = (float)xGamepad.sThumbRX / SHRT_MAX;
//         gamepad_.rightStickY_ = (float)xGamepad.sThumbRY / SHRT_MAX;
//         if (gamepad_.leftStickX_ < 0.1f && gamepad_.leftStickX_ > -0.1f)
//             gamepad_.leftStickX_ = 0.0f;
//         if (gamepad_.leftStickY_ < 0.1f && gamepad_.leftStickY_ > -0.1f)
//             gamepad_.leftStickY_ = 0.0f;
//         if (gamepad_.rightStickX_ < 0.1f && gamepad_.rightStickX_ > -0.1f)
//             gamepad_.rightStickX_ = 0.0f;
//         if (gamepad_.rightStickY_ < 0.1f && gamepad_.rightStickY_ > -0.1f)
//             gamepad_.rightStickY_ = 0.0f;
//
//         gamepad_.SetButtonHeld(GAMEPAD_A, xGamepad.wButtons & XINPUT_GAMEPAD_A);
//         gamepad_.SetButtonHeld(GAMEPAD_B, xGamepad.wButtons & XINPUT_GAMEPAD_B);
//         gamepad_.SetButtonHeld(GAMEPAD_X, xGamepad.wButtons & XINPUT_GAMEPAD_X);
//         gamepad_.SetButtonHeld(GAMEPAD_Y, xGamepad.wButtons & XINPUT_GAMEPAD_Y);
//         gamepad_.SetButtonHeld(GAMEPAD_UP, xGamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
//         gamepad_.SetButtonHeld(GAMEPAD_DOWN, xGamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
//         gamepad_.SetButtonHeld(GAMEPAD_LEFT, xGamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
//         gamepad_.SetButtonHeld(GAMEPAD_RIGHT, xGamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
//         gamepad_.SetButtonHeld(GAMEPAD_LSHOULDER, xGamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
//         gamepad_.SetButtonHeld(GAMEPAD_RSHOULDER, xGamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
//         gamepad_.SetButtonHeld(GAMEPAD_LTHUMB, xGamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
//         gamepad_.SetButtonHeld(GAMEPAD_RTHUMB, xGamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
//         gamepad_.SetButtonHeld(GAMEPAD_START, xGamepad.wButtons & XINPUT_GAMEPAD_START);
//         gamepad_.SetButtonHeld(GAMEPAD_SELECT, xGamepad.wButtons & XINPUT_GAMEPAD_BACK);
//
//         gamepad_.leftTrigger_ = (float)xGamepad.bLeftTrigger / MAXBYTE;
//         gamepad_.rightTrigger_ = (float)xGamepad.bRightTrigger / MAXBYTE;
//
//         gamepad_.SetButtonHeld(GAMEPAD_LTRIGGER, gamepad_.leftTrigger_ > TRIGGER_LIMIT);
//         gamepad_.SetButtonHeld(GAMEPAD_RTRIGGER, gamepad_.rightTrigger_ > TRIGGER_LIMIT);
//     }
}
//
void Game::UpdateInputs_P() {
    inputs_.forwardInput = 0.0f;
    inputs_.sideInput = 0.0f;

    inputs_.cut = platform_.heldKeys_[' '];// || gamepad_.heldButtons_[GAMEPAD_Y];
    inputs_.ski = platform_.heldKeys_[RIGHT_MOUSE_KEY];// || gamepad_.heldButtons_[GAMEPAD_RTRIGGER] || gamepad_.heldButtons_[GAMEPAD_RSHOULDER];
    inputs_.flow = platform_.heldKeys_[LEFT_MOUSE_KEY];// || gamepad_.heldButtons_[GAMEPAD_LTRIGGER] || gamepad_.heldButtons_[GAMEPAD_LSHOULDER]; 

    // TODO: clamp the values to total size 1
    if (platform_.heldKeys_['W'])
        inputs_.forwardInput += 1.0f;

    if (platform_.heldKeys_['S'])
        inputs_.forwardInput -= 1.0f;

    if (platform_.heldKeys_['D'])
        inputs_.sideInput += 1.0f;

    if (platform_.heldKeys_['A'])
        inputs_.sideInput -= 1.0f;

    // inputs_.forwardInput += gamepad_.leftStickY_;
    // inputs_.sideInput += gamepad_.leftStickX_;

    // TODO: scale these by delta time and clamp the values
    inputs_.deltaLookX = 0.0f;
    inputs_.deltaLookY = 0.0f;
    inputs_.deltaLookX -= platform_.deltaMouseX_ * 0.005f;
    inputs_.deltaLookY -= platform_.deltaMouseY_ * 0.005f;
    // inputs_.deltaLookX -= gamepad_.rightStickX_ * Time::GetDeltaTime() * 2.0f;
    // inputs_.deltaLookY += gamepad_.rightStickY_ * Time::GetDeltaTime() * 2.0f;
}

void Game::FlushInputs_P() {
    // Have to toggle on flush since released keys are cleared per game update
    // and not per program update. Clearing happens in this function
    #ifdef _DEBUG
    if (platform_.pressedKeys_['Q'])
        ScreenText::Toggle();
    
    if (platform_.pressedKeys_[GLFW_KEY_ESCAPE])
        running_ = false;
    #endif

    platform_.FlushKeys();
    gamepad_.pressedButtons_.reset();
    gamepad_.releasedButtons_.reset();
}
