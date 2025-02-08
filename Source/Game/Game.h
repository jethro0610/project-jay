#pragma once
#include "Camera/Camera.h"
#include "Currency.h"
#include "Types/Inputs.h"
#include "Particle/ParticleManager.h"
#include "Seed/SeedManager.h"
#include "Spread/SpreadManager.h"
#include "Rendering/Renderer.h"
#include "Resource/ResourceManager.h"
#include "Level/Level.h"
#include "Terrain/Terrain.h"
#include "Entity/EntityUnion.h"
#include "Entity/EntityList.h"

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
    Renderer renderer_;
    Camera camera_;
    Currency currency_;
    SeedManager seedManager_;
    SpreadManager spreadManager_;

    EntityUnion rawEntities_[EntityList::MAX];
    EntityList entities_;

    #ifdef _DEBUG
    Editor editor_;
    #endif

    Inputs inputs_;

    float timeAccumlulator_;

    void UpdateInputs_P();
    void PollGamepadInputs_P();
    void FlushInputs_P();
};
