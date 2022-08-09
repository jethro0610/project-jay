#pragma once

#ifdef WINDOWS
#include "Windows/WindowsLayer.h"
#include "Rendering/DirectXLayer.h"
#endif

#include "Gamepad.h"
#include "Rendering/Renderer.h"

class Game {
public:
    Game();
    ~Game();
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
    DirectXLayer* dxLayer_;
    #endif

    void UpdateInputs();
    void PollGamepadInputs();
};