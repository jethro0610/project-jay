#include <glm/gtx/projection.hpp>
#include "Ray.h"
#include "Logging/Logger.h"
using namespace glm;

bool Ray::RayHitCollider(
    glm::vec3 origin,
    glm::vec3 direction,
    const Transform& transform,
    const Collider& collider
) {
    float scale = (transform.scale.x + transform.scale.z) * 0.5f;

    vec3 vectorToCollider = transform.position - origin; 

    vec3 projected = glm::proj(vectorToCollider, direction);
    vec3 projectedPosition = origin + projected;

    vec2 colliderOrigin2D = vec2(transform.position.x, transform.position.z);
    vec2 projectedPosition2D = vec2(projectedPosition.x, projectedPosition.z);

    if (distance(colliderOrigin2D, projectedPosition2D) > collider.radius * scale)
        return false;

    if (projectedPosition.y > transform.position.y + collider.top * transform.scale.y)
        return false;

    if (projectedPosition.y < transform.position.y - collider.bottom * transform.scale.y)
        return false;

    return true;
}

bool Ray::RayHitSphere(
    glm::vec3 origin,
    glm::vec3 direction,
    glm::vec3 sphereOrigin,
    float radius
) {
    vec3 vectorToSphere = sphereOrigin - origin; 

    vec3 projected = glm::proj(vectorToSphere, direction);
    vec3 projectedPosition = origin + projected;

    if (distance(sphereOrigin, projectedPosition) > radius)
        return false;
    else
        return true;
}
