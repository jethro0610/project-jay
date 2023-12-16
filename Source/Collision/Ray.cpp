#include <glm/gtx/projection.hpp>
#include "Ray.h"
using namespace glm;

bool Ray::RayHitCollider(
    glm::vec3 origin,
    glm::vec3 direction,
    const Transform& transform,
    const Collider& collider
) {
    vec2 rayOrigin2D = vec2(origin.x, origin.z);
    vec2 colliderOrigin2D = vec2(transform.position.x, transform.position.z);
    vec2 direction2D = vec2(direction.x, direction.z);
    float scale = (transform.scale.x + transform.scale.z) * 0.5f;

    vec2 vectorToCollider = colliderOrigin2D - rayOrigin2D; 

    vec2 projected = glm::proj(vectorToCollider, direction2D);
    vec2 projectedPosition = rayOrigin2D + projected;

    if (distance(colliderOrigin2D, projectedPosition) <= collider.radius * scale)
        return true;
    else
        return false;
}
