#pragma once
#include "./Entity/EntityManager.h"
#include "Camera.h"

const int CUT_TIME = 60;

class PlayerController {
public:
    int actionMeter_;
    int cutTimer_;

    EntityManager& entityManager_;
    Camera& camera_;

    PlayerController(EntityManager& entityManager, Camera& camera);

    void Execute(Inputs inputs);
};
