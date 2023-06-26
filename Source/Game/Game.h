#pragma once
#include "Camera.h"
#include "Entity/EntityManager.h"
#include "../Types/Gamepad.h"
#include "../Types/Inputs.h"
#include "PlayerController.h"
#include "World/SeedManager.h"
#include "World/SpreadManager.h"
#include "Systems/Systems.h"
#include "../Types/Transform.h"
#include "../Rendering/Renderer.h"
#include "World/World.h"

#ifdef _PC
#include "../Platform/PC_Platform.h"
#endif

class Game {
public:
    Game();
    void Init();

    bool running_;
    void Update();

private:
    Platform platform_;

    EntityManager entityManager_;
    Renderer renderer_;
    Camera camera_;
    World world_;
    SeedManager seedManager_;
    SpreadManager spreadManager_;
    PlayerController playerController_;

    Inputs inputs_;

    float timeAccumlulator_;

    void UpdateInputs_P();
    void PollGamepadInputs_P();
    void FlushInputs_P();
};
