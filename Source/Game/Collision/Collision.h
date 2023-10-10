#pragma once
#include "./Collider.h"
#include "../../Types/Transform.h"

struct Collision {
    bool isColliding;
    glm::vec3 resolution;

    Collision() {
        isColliding = false;
        resolution = glm::vec3(0.0f);
    }

    static Collision GetCollision(
        const Transform& transformA,
        const Collider& colA, 
        const Transform& transformB,
        const Collider& colB
    );
};

