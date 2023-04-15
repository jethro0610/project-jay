#pragma once
#include "Entity/EntityManager.h"
#include "World/World.h"
#include "World/SpreadManager.h"
#include "Camera.h"

const uint8_t TIME_TO_CUT = 15;
const uint16_t MAX_ACTION_METER = 250;

class PlayerController {
public:
    PlayerController(EntityManager& entityManager, World& world, SpreadManager& spreadManager, Camera& camera);
    void Execute(Inputs inputs);

private:
    uint16_t actionMeter_;
    uint8_t cutTimer_;
    uint8_t cutCooldown_;

    EntityManager& entityManager_;
    World& world_;
    SpreadManager& spreadManager_;
    Camera& camera_;
};
