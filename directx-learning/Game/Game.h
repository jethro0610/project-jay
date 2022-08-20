#pragma once

#ifdef WINDOWS
#include "../Windows/WindowsLayer.h"
#include "../Resource/DXResources.h"
#endif

#include "World.h"
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

    // TODO: convert these to vec2
    float forwardInput_;
    float sideInput_;
    float deltaLookX_;
    float deltaLookY_;
    float lookX_;
    float lookY_;

    int resolutionWidth_;
    int resolutionHeight_;

    Gamepad gamepad_;
    void UpdateCameraTransform();

    World* world_;
    void SendWorldMeshToGPU_P(ivec3 coordinates, const std::vector<WorldVertex>& vertices, const std::vector<uint16_t> indices);

private:
    Renderer* renderer_;
    #ifdef WINDOWS
    WindowsLayer* windowsLayer_;
    DXResources* dxResources_;
    #endif

    void UpdateInputs_P();
    void PollGamepadInputs_P();
};