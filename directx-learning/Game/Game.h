#pragma once

#define MAX_DELTA_TIME 1.0f/15.0f

#ifdef WINDOWS
#include "../Windows/WindowsLayer.h"
#include "../Resource/DXResources.h"
#endif

#include <chrono>
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

    // Called during Init_P right before the game loop starts
    void Init();

    // Called before Init to start the game
    void Init_P();

    bool running_;
    void Update(float deltaTime, float elapsedTime);

    // TODO: convert these to vec2
    Gamepad gamepad_;
    float forwardInput_;
    float sideInput_;
    float deltaLookX_;
    float deltaLookY_;

    int resolutionWidth_;
    int resolutionHeight_;

    long long lastTimeUSec_;
    long long currentTimeUSec_;
    float deltaTime_;
    float elapsedTime_;

    ActiveEntityComponents activeEntityC_;
    TransformComponents transformC_;
    ColliderComponents colliderC_;
    StaticModelComponents staticModelC_;

    Camera* camera_;

    World* world_;
    void SendWorldMeshToGPU_P(ivec3 coordinates, const std::vector<WorldVertex>& vertices, const std::vector<uint16_t> indices);

private:
    Renderer* renderer_;
    ResourceManager* resourceManager_;

    #ifdef WINDOWS
    WindowsLayer* windowsLayer_;
    DXResources* dxResources_;
    #endif

    void UpdateInputs_P(float deltaTime);
    void PollGamepadInputs_P();
    void UpdateTime();
};