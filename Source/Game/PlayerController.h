#pragma once
#include <stdint.h>
#include "../Types/Inputs.h"
#include "ParticleManager.h"
#include "./Components/ComponentList.h"

class Camera;
class SpreadManager;
class World;

const int TIME_TO_CUT = 16;
const int CUT_COOLDOWN_TIME = 16;
const int MAX_ACTION_METER = 250;

const int HIGH_TIME = 15;
const int STRONG_TIME = 15;

class PlayerController {
public:
    PlayerController();
    void Init(
        ComponentList& components,
        ParticleManager& particleManager
    );

    void Execute(
        ComponentList& components,
        World& world, 
        SpreadManager& spreadManager, 
        Camera& camera,
        Inputs inputs
    );

private:
    int actionMeter_;
    int cutTimer_;
    int cutCooldown_;
    ParticleEmitter* dustEmitter_;
    ParticleEmitter* cloudEmitter_;
    ParticleEmitter* sparkEmitter_;

    int highCooldown_;
    int strongCooldown_;
};
