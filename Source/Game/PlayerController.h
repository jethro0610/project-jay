#pragma once
#include "./Entity/EntityManager.h"
#include "Camera.h"

const uint8_t TIME_TO_CUT = 15;

class PlayerController {
public:
    uint8_t actionMeter_;
    uint8_t cutTimer_;

    EntityManager& entityManager_;
    Camera& camera_;

    PlayerController(EntityManager& entityManager, Camera& camera);

    void Execute(Inputs inputs);
};
