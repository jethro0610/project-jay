#pragma once
#include "Collider.h"
#include "Types/Transform.h"

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;

    Ray(const glm::vec3& inOrigin, const glm::vec3& inDirection) {
        origin = inOrigin;
        direction = inDirection;
    };

    bool HitCollider (
        const Collider& collider,
        const Transform& transform
    ) const;

    bool HitSphere(
        const glm::vec3& sphereOrigin,
        float radius
    ) const;
};
