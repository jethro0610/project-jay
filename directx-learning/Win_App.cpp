#include "App.h"
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
        inputHandler.Poll();
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

    forwardInput_ += inputHandler.leftStickY_;
    sideInput_ += inputHandler.leftStickX_;

    std::stringstream ss;
    ss << "Forward: " << forwardInput_ << " Side: " << sideInput_;
    SetWindowText(windowsLayer_->windowHandle_, ss.str().c_str());
}