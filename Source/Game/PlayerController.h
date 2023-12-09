#pragma once
#include "Game/Systems/SystemInc.h"
#include "Types/Inputs.h"

class Camera;
class SpreadManager;
class Terrain;

class PlayerController {
public:
    static const int TIME_TO_CUT = 16;
    static const int CUT_COOLDOWN_TIME = 16;
    static const int MAX_ACTION_METER = 250;

    static const int ATTACK_TIME = 25;
    static const int ATTACK_ACTIVE_START = 2;
    static const int ATTACK_ACTIVE_END = 15;

    static const int MAX_CHARGE = 75;
    static const int STRONG_CHARGE = 15;

    static const int DUST_EMITTER = 0;
    static const int CLOUD_EMITTER = 1;
    static const int SPARK_EMITTER = 2;

    PlayerController();

    void Execute(
        EntityList& entitities,
        ComponentList& components,
        Terrain& terrain, 
        SpreadManager& spreadManager, 
        Camera& camera,
        Inputs inputs
    );

private:
    int actionMeter_;
    int cutTimer_;
    int cutCooldown_;

    int attackTimer_;
    int attackHitbox_;
    int charge_;
    bool releasedCharge_;
};
