#pragma once
#include "Camera/Camera.h"
#include "Entity/EntityManager.h"
#include "Types/Inputs.h"
#include "Controller/PlayerController.h"
#include "Particle/ParticleManager.h"
#include "Seed/SeedManager.h"
#include "Spread/SpreadManager.h"
#include "Systems/Systems.h"
#include "Rendering/Renderer.h"
#include "Level/LevelLoader.h"
#include "Terrain/Terrain.h"

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
    ResourceManager resourceManager_;
    ParticleManager particleManager_;
    EntityManager entityManager_;
    LevelLoader levelLoader_;
    Renderer renderer_;
    Camera camera_;
    Terrain terrain_;
    SeedManager seedManager_;
    SpreadManager spreadManager_;
    PlayerController playerController_;

    Inputs inputs_;

    float timeAccumlulator_;

    void UpdateInputs_P();
    void PollGamepadInputs_P();
    void FlushInputs_P();
};
