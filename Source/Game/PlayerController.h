#pragma once
#include <stdint.h>
#include "../Types/Inputs.h"
class Camera;
class SpreadManager;
class World;
class GroundTraceComponent;
class MovementComponent;
class SpreadActivatorComponent;
class TransformComponent;
class VelocityComponent;

const uint8_t TIME_TO_CUT = 16;
const uint8_t CUT_COOLDOWN_TIME = 32;
const uint16_t MAX_ACTION_METER = 250;

class PlayerController {
public:
    PlayerController();
    void Execute(
        World& world, 
        SpreadManager& spreadManager, 
        Camera& camera,
        GroundTraceComponent& groundTraceComponent,
        MovementComponent& movementComponent,
        SpreadActivatorComponent& spreadActivatorComponent,
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent,
        Inputs inputs
    );

private:
    uint32_t seedMeter_;
    uint16_t actionMeter_;
    uint8_t cutTimer_;
    uint8_t cutCooldown_;
};
