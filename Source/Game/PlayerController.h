#pragma once
#include "./Entity/EntityManager.h"
#include "./SpreadManager.h"
#include "Camera.h"

const uint8_t TIME_TO_CUT = 15;
const uint16_t MAX_ACTION_METER = 250;

class PlayerController {
public:
    uint16_t actionMeter_;
    uint8_t cutTimer_;
    uint8_t cutCooldown_;

    EntityManager& entityManager_;
    SpreadManager& spreadManager_;
    Camera& camera_;

    PlayerController(EntityManager& entityManager, SpreadManager& spreadManager_, Camera& camera);

    void Execute(Inputs inputs);
};
