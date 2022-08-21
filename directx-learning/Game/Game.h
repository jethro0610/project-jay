#pragma once

#ifdef WINDOWS
#include "../Windows/WindowsLayer.h"
#include "../Resource/DXResources.h"
#endif

#include "World.h"
#include "../Resource/ResourceManager.h"
#include "../Types/Gamepad.h"
#include "../Types/Transform.h"
#include "../Rendering/Renderer.h"


#include "Components/Components.h"
#include "Systems/Systems.h"

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

    ActiveEntityComponents activeEntityC_;
    TransformComponents transformC_;
    ColliderComponents colliderC_;
    StaticModelComponents staticModelC_;

    Gamepad gamepad_;
    void UpdateCameraTransform();

    World* world_;
    void SendWorldMeshToGPU_P(ivec3 coordinates, const std::vector<WorldVertex>& vertices, const std::vector<uint16_t> indices);

private:
    Renderer* renderer_;
    ResourceManager* resourceManager_;

    #ifdef WINDOWS
    WindowsLayer* windowsLayer_;
    DXResources* dxResources_;
    #endif

    void UpdateInputs_P();
    void PollGamepadInputs_P();
};