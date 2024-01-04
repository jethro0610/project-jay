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
#include "Level/LevelProperties.h"
#include "Level/LevelLoader.h"
#include "Terrain/Terrain.h"
#include "Entity/Player.h"

#ifdef _PC
#include "Platform/PC_Platform.h"
#endif

#ifdef _DEBUG
#include "Editor/Editor.h"
#endif

class Game {
public:
    Game();
    void Init();

    bool running_;
    void Update();
    void EditModeUpdate();

private:
    Platform platform_;
    LevelProperties levelProperties_;
    ResourceManager resourceManager_;
    ParticleManager particleManager_;
    Terrain terrain_;
    EntityManager entityManager_;
    LevelLoader levelLoader_;
    Renderer renderer_;
    Camera camera_;
    SeedManager seedManager_;
    SpreadManager spreadManager_;
    PlayerController playerController_;
    #ifdef _DEBUG
    Editor editor_;
    #endif

    Player testPlayer_;

    Inputs inputs_;

    float timeAccumlulator_;

    void UpdateInputs_P();
    void PollGamepadInputs_P();
    void FlushInputs_P();
};
