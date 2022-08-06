#include "App.h"
#include <sstream>

App::App() {
    running_ = true;
    windowsLayer_ = nullptr;
    forwardInput_ = 0;
    sideInput_ = 0;
}

void App::Run() {
    windowsLayer_ = WindowsLayer::InitWindowsLayer(800, 600, "DirectX Learning");
    MSG msg;
    while (!windowsLayer_->closed_ && running_) {
        windowsLayer_->ClearPressedAndReleasedKeys();
        windowsLayer_->ClearMouseMovement();

        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        windowsLayer_->UpdateMouseMovement();
        UpdateInputs();
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
}