#pragma once
#ifdef WINDOWS
#include "../Windows/WindowsLayer.h"
#include "../Resource/DXResources.h"
#endif

#include <chrono>
#include "TimeConstants.h"
#include "World.h"
#include "../Resource/ResourceManager.h"
#include "../Types/Gamepad.h"
#include "../Types/Transform.h"
#include "../Rendering/Renderer.h"
#include "Components/Components.h"
#include "Systems/Systems.h"
#include "../Types/Inputs.h"
#include "EntityManager.h"

class Game {
public:
    Game();

    // Called during Init_P right before the game loop starts
    void Init();

    // Called before Init to start the game
    void Init_P();

    bool running_;
    void Update(float deltaTime, float elapsedTime);

private:
#ifdef WINDOWS
    WindowsLayer* windowsLayer_;
    DXResources* dxResources_;
#endif

    Renderer* renderer_;
    ResourceManager* resourceManager_;
    Camera* camera_;
    World* world_;

    Gamepad gamepad_;
    Inputs inputs_;

    int resolutionWidth_;
    int resolutionHeight_;

    long long lastTimeUSec_;
    long long currentTimeUSec_;
    float deltaTime_;
    float elapsedTime_;
    float timeAccumlulator_;

    EntityManager entityManager_;

    void SendWorldMeshToGPU_P(ivec3 coordinates, const std::vector<WorldVertex>& vertices, const std::vector<uint16_t> indices);
    void UpdateInputs_P(float deltaTime);
    void PollGamepadInputs_P();
    void UpdateTime();
};