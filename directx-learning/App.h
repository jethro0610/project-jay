#pragma once

#ifdef WINDOWS
#include "WindowsLayer.h"
#include "DirectXLayer.h"
#endif

#include "Gamepad.h"
#include "Renderer.h"

class App {
public:
    App();
    ~App();
    void PlatformInit();
    void Init();

    bool running_;
    void Update();

    float forwardInput_;
    float sideInput_;

    int resolutionWidth_;
    int resolutionHeight_;

    Gamepad gamepad;

private:
    Renderer* renderer_;
    #ifdef WINDOWS
    WindowsLayer* windowsLayer_;
    DirectXLayer* dxLayer;
    #endif

    void UpdateInputs();
    void PollGamepadInputs();
};