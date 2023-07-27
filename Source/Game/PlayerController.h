#pragma once
#include <stdint.h>
#include "../Types/Inputs.h"
class Camera;
class SpreadManager;
class World;
class GroundTraceComponent;
class MeterComponent;
class MovementComponent;
class SpreadActivatorComponent;
class TransformComponent;
class VelocityComponent;

const int TIME_TO_CUT = 16;
const int CUT_COOLDOWN_TIME = 16;
const int MAX_ACTION_METER = 300;

class PlayerController {
public:
    PlayerController();
    void Execute(
        World& world, 
        SpreadManager& spreadManager, 
        Camera& camera,
        GroundTraceComponent& groundTraceComponent,
        MeterComponent& meterComponent,
        MovementComponent& movementComponent,
        SpreadActivatorComponent& spreadActivatorComponent,
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent,
        Inputs inputs
    );

private:
    int actionMeter_;
    int cutTimer_;
    int cutCooldown_;
};
