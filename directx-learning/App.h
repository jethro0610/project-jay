#pragma once

#ifdef WINDOWS
#include "WindowsLayer.h"
#endif

#include "Gamepad.h"
#include "Rendering.h"

class App {
public:
    App();

    bool running_;
    void Run();

    float forwardInput_;
    float sideInput_;

    int resolutionWidth;
    int resolutionHeight;

    Gamepad gamepad;

private:
    Rendering* renderer;
    #ifdef WINDOWS
    WindowsLayer* windowsLayer_;
    #endif

    void UpdateInputs();
    void PollGamepadInputs();
};