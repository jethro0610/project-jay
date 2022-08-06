#pragma once

#ifdef WINDOWS
#include "WindowsLayer.h"
#endif

#include "Gamepad.h"

class App {
public:
    App();

    bool running_;
    void Run();

    float forwardInput_;
    float sideInput_;

    Gamepad gamepad;

private:
    #ifdef WINDOWS
    WindowsLayer* windowsLayer_;
    #endif

    void UpdateInputs();
    void PollGamepadInputs();
};