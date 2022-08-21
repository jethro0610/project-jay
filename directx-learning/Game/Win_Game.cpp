#include "Game.h"
#include <Xinput.h>
#include <sstream>

using namespace std::chrono;

Game::~Game() {
    delete resourceManager_;
    delete dxResources_;
    delete renderer_;
    delete windowsLayer_;
}

void Game::Init_P() {
    running_ = true;
    windowsLayer_ = WindowsLayer::InitWindowsLayer(resolutionWidth_, resolutionHeight_, "DirectXLearning");
    dxResources_ = new DXResources(windowsLayer_->windowHandle_, resolutionWidth_, resolutionHeight_);
    resourceManager_ = new ResourceManager(dxResources_);
    renderer_ = new Renderer(camera_, resourceManager_);
    Init();

    MSG msg;
    while (!windowsLayer_->closed_ && running_) {
        UpdateTime();

        windowsLayer_->ClearPressedAndReleasedKeys();
        windowsLayer_->ClearMouseMovement();

        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        windowsLayer_->UpdateMouseMovement();
        PollGamepadInputs_P();
        UpdateInputs_P(deltaTime_);
        Update(deltaTime_, elapsedTime_);
    }
}

void Game::PollGamepadInputs_P() {
    XINPUT_STATE state;

    gamepad_.pressedButtons_.reset();
    gamepad_.releasedButtons_.reset();
    gamepad_.leftStickX_ = 0.0f;
    gamepad_.leftStickY_ = 0.0f;
    gamepad_.rightStickX_ = 0.0f;
    gamepad_.rightStickY_ = 0.0f;
    gamepad_.leftTrigger_ = 0.0f;
    gamepad_.rightTrigger_ = 0.0f;
    for (DWORD controllerIndex = 0; controllerIndex < XUSER_MAX_COUNT; controllerIndex++) {
        if (XInputGetState(0, &state) != ERROR_SUCCESS) continue;
        XINPUT_GAMEPAD xGamepad = state.Gamepad;

        gamepad_.leftStickX_ = (float)xGamepad.sThumbLX / SHRT_MAX;
        gamepad_.leftStickY_ = (float)xGamepad.sThumbLY / SHRT_MAX;
        gamepad_.rightStickX_ = (float)xGamepad.sThumbRX / SHRT_MAX;
        gamepad_.rightStickY_ = (float)xGamepad.sThumbRY / SHRT_MAX;

        gamepad_.SetButtonHeld(GAMEPAD_A, xGamepad.wButtons & XINPUT_GAMEPAD_A);
        gamepad_.SetButtonHeld(GAMEPAD_B, xGamepad.wButtons & XINPUT_GAMEPAD_B);
        gamepad_.SetButtonHeld(GAMEPAD_X, xGamepad.wButtons & XINPUT_GAMEPAD_X);
        gamepad_.SetButtonHeld(GAMEPAD_Y, xGamepad.wButtons & XINPUT_GAMEPAD_Y);
        gamepad_.SetButtonHeld(GAMEPAD_UP, xGamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
        gamepad_.SetButtonHeld(GAMEPAD_DOWN, xGamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
        gamepad_.SetButtonHeld(GAMEPAD_LEFT, xGamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
        gamepad_.SetButtonHeld(GAMEPAD_RIGHT, xGamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
        gamepad_.SetButtonHeld(GAMEPAD_LSHOULDER, xGamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
        gamepad_.SetButtonHeld(GAMEPAD_RSHOULDER, xGamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
        gamepad_.SetButtonHeld(GAMEPAD_LTHUMB, xGamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
        gamepad_.SetButtonHeld(GAMEPAD_RTHUMB, xGamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
        gamepad_.SetButtonHeld(GAMEPAD_START, xGamepad.wButtons & XINPUT_GAMEPAD_START);
        gamepad_.SetButtonHeld(GAMEPAD_SELECT, xGamepad.wButtons & XINPUT_GAMEPAD_BACK);

        gamepad_.leftTrigger_ = (float)xGamepad.bLeftTrigger / MAXBYTE;
        gamepad_.rightTrigger_ = (float)xGamepad.bRightTrigger / MAXBYTE;

        gamepad_.SetButtonHeld(GAMEPAD_LTRIGGER, gamepad_.leftTrigger_ > gamepad_.triggerLimit_);
        gamepad_.SetButtonHeld(GAMEPAD_RTRIGGER, gamepad_.rightTrigger_ > gamepad_.triggerLimit_);
    }
}

void Game::UpdateInputs_P(float deltaTime) {
    forwardInput_ = 0.0f;
    sideInput_ = 0.0f;

    // TODO: clamp the values to total size 1
    if (windowsLayer_->heldKeys_['W'])
        forwardInput_ += 1.0f;

    if (windowsLayer_->heldKeys_['S'])
        forwardInput_ -= 1.0f;

    if (windowsLayer_->heldKeys_['D'])
        sideInput_ += 1.0f;

    if (windowsLayer_->heldKeys_['A'])
        sideInput_ -= 1.0f;

    forwardInput_ += gamepad_.leftStickY_;
    sideInput_ += gamepad_.leftStickX_;

    // TODO: scale these by delta time and clamp the values
    deltaLookX_ = 0.0f;
    deltaLookY_ = 0.0f;
    deltaLookX_ -= windowsLayer_->deltaMouseX_ * 0.005f;
    deltaLookY_ -= windowsLayer_->deltaMouseY_ * 0.005f;
    deltaLookX_ -= gamepad_.rightStickX_ * deltaTime * 2.0f;
    deltaLookY_ += gamepad_.rightStickY_ * deltaTime * 2.0f;
}

void Game::SendWorldMeshToGPU_P(ivec3 coordinates, const std::vector<WorldVertex>& vertices, const std::vector<uint16_t> indices) {
    dxResources_->WriteWorldMesh(coordinates, vertices, indices);
}