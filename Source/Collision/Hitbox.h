#pragma once
#include "Collider.h"
#include <glm/vec3.hpp>

struct Hitbox : public Collider {
    bool active;
    glm::vec3 knocback;
    float forwardRange;
    float diStrength;

    int hitlag;
    int damage;
};
