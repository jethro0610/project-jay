#pragma once
#ifdef _WINDOWS
#include "../Windows/WindowsLayer.h"
#include "../Resource/DXResources.h"
#endif

#include <chrono>
#include "Camera.h"
#include "Entity/EntityManager.h"
#include "../Types/Gamepad.h"
#include "../Types/Inputs.h"
#include "PlayerController.h"
#include "World/SpreadManager.h"
#include "Systems/Systems.h"
#include "../Types/Transform.h"
#include "../Rendering/Renderer.h"
#include "../Resource/ResourceManager.h"
#include "World/World.h"

class Game {
public:
    Game(int width, int height);
    void Init();

    bool running_;
    void Update(float deltaTime, float elapsedTime);

private:
#ifdef _WINDOWS
    WindowsLayer windowsLayer_;
    DXResources dxResources_;
#endif
    ResourceManager resourceManager_;
    EntityManager entityManager_;
    Renderer renderer_;
    Camera camera_;
    World world_;
    SpreadManager spreadManager_;
    PlayerController playerController_;

    Gamepad gamepad_;
    Inputs inputs_;

    int resolutionWidth_;
    int resolutionHeight_;

    long long lastTimeUSec_;
    long long currentTimeUSec_;
    float deltaTime_;
    float elapsedTime_;
    float timeAccumlulator_;

    void UpdateInputs_P(float deltaTime);
    void PollGamepadInputs_P();
    void FlushInputs_P();
    void UpdateTime();
};
