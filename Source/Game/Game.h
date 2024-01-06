#pragma once
#include "Camera/Camera.h"
#include "Types/Inputs.h"
#include "Particle/ParticleManager.h"
#include "Seed/SeedManager.h"
#include "Spread/SpreadManager.h"
#include "Rendering/Renderer.h"
#include "Level/LevelProperties.h"
#include "Level/LevelLoader.h"
#include "Terrain/Terrain.h"

#include "Entity/EntityListS.h"
#include "Entity/EntityUnion.h"

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
    LevelLoader levelLoader_;
    Renderer renderer_;
    Camera camera_;
    SeedManager seedManager_;
    SpreadManager spreadManager_;

    EntityUnion entities_[128];
    EntityListS entityListS_;

    #ifdef _DEBUG
    Editor editor_;
    #endif

    Inputs inputs_;

    float timeAccumlulator_;

    void UpdateInputs_P();
    void PollGamepadInputs_P();
    void FlushInputs_P();
};
