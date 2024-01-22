#pragma once
#include "Collider.h"
#include "Types/Transform.h"

namespace Ray {
    bool RayHitCollider(
        glm::vec3 origin,
        glm::vec3 direction,
        const Transform& transform,
        const Collider& collider
    );

    bool RayHitSphere(
        glm::vec3 origin,
        glm::vec3 direction,
        glm::vec3 sphereOrigin,
        float radius
    );
}
