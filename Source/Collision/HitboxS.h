#pragma once
#include "Collider.h"

struct HitboxS : public Collider {
    bool active;
    float directionInfluence;
    float horizontalKb;
    float verticalKb;
    float forwardRange;

    int hitlag;
    int damage;
};
