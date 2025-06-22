#pragma once
#include "Camera/Camera.h"
#include "Clock.h"
#include "Types/Inputs.h"
#include "Particle/ParticleManager.h"
#include "Seed/SeedManager.h"
#include "Spread/SpreadManager.h"
#include "Rendering/Renderer.h"
#include "Resource/ResourceManager.h"
#include "Level/Level.h"
#include "LevelControllers/ControllerUnion.h"
#include "Terrain/Terrain.h"
#include "Entity/EntityUnion.h"
#include "Entity/EntityList.h"
#include "ScoreKeeper.h"
#include "WaterManager.h"

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
    ResourceManager resourceManager_;
    ParticleManager particleManager_;
    Terrain terrain_;
    Level level_;
    ControllerUnion levelController_;
    Renderer renderer_;
    Camera camera_;
    Clock clock_;
    ScoreKeeper scoreKeeper_;
    SeedManager seedManager_;
    SpreadManager spreadManager_;
    WaterManager waterManager_;

    EntityUnion rawEntities_[EntityList::MAX];
    EntityList entities_;

    #ifdef _DEBUG
    Editor editor_;
    #endif

    Inputs inputs_;
    #ifdef _DEBUG
    Inputs cameraInputs_;
    #endif

    float timeAccumlulator_;

    void UpdateInputs_P();
    void PollGamepadInputs_P();
    void FlushInputs_P();
};
