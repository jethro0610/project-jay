#pragma once

#ifdef WINDOWS
#include "../Windows/WindowsLayer.h"
#include "../Resource/DXResources.h"
#endif

#include "../Types/Gamepad.h"
#include "../Types/Transform.h"
#include "../Rendering/Renderer.h"

class Game {
public:
    Game();
    ~Game();
    void Init();
    void Init_P();

    bool running_;
    void Update();

    float forwardInput_;
    float sideInput_;
    float deltaLookX_;
    float deltaLookY_;
    float lookX_;
    float lookY_;

    int resolutionWidth_;
    int resolutionHeight_;

    Gamepad gamepad;
    void UpdateCameraTransform();

private:
    Renderer* renderer_;
    #ifdef WINDOWS
    WindowsLayer* windowsLayer_;
    DXResources* dxResources_;
    #endif

    void UpdateInputs_P();
    void PollGamepadInputs_P();
};