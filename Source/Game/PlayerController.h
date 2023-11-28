#pragma once
#include <stdint.h>
#include "../Types/Inputs.h"
#include "ParticleManager.h"
#include "./Components/ComponentList.h"
#include "./Entity/EntityList.h"

class Camera;
class SpreadManager;
class World;

const int TIME_TO_CUT = 16;
const int CUT_COOLDOWN_TIME = 16;
const int MAX_ACTION_METER = 250;

const int ATTACK_TIME = 25;
const int ATTACK_ACTIVE_START = 2;
const int ATTACK_ACTIVE_END = 15;

const int MAX_CHARGE = 75;
const int STRONG_CHARGE = 15;

class PlayerController {
public:
    PlayerController();
    void Init(
        ComponentList& components,
        ParticleManager& particleManager
    );

    void Execute(
        EntityList& entitities,
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

    int attackTimer_;
    int attackHitbox_;
    int charge_;
    bool releasedCharge_;
};
