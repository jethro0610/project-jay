#pragma once
#include "./Entity/EntityManager.h"
#include "Camera.h"

const int TIME_TO_CUT = 15;

class PlayerController {
public:
    int actionMeter_;
    int cutTimer_;

    EntityManager& entityManager_;
    Camera& camera_;

    PlayerController(EntityManager& entityManager, Camera& camera);

    void Execute(Inputs inputs);
};
