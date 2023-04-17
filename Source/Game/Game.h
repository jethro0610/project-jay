#pragma once
#ifdef _WINDOWS
#include "../Windows/WindowsLayer.h"
#include "../Resource/DXResources.h"
#endif

#include <chrono>
#include "World/World.h"
#include "../Resource/ResourceManager.h"
#include "../Types/Gamepad.h"
#include "../Types/Transform.h"
#include "../Types/Inputs.h"
#include "../Rendering/Renderer.h"
#include "Components/ComponentInclude.h"
#include "Systems/Systems.h"
#include "World/SpreadManager.h"
#include "Entity/EntityManager.h"
#include "PlayerController.h"

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
    PlayerController playerController_;
    SpreadManager& spreadManager_;

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
