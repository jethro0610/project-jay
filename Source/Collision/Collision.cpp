#include "Collision.h"
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/string_cast.hpp>
using namespace glm;

Collision Collision::GetCollision(
    const Transform& transformA,
    const Collider& colA, 
    const Transform& transformB,
    const Collider& colB
) {
    Collision collision;
    vec2 posA2 = glm::vec2(transformA.position.x, transformA.position.z);
    vec2 posB2 = glm::vec2(transformB.position.x, transformB.position.z);
    float pScaleA = (transformA.scale.x + transformA.scale.z) * 0.5f;
    float pScaleB = (transformB.scale.x + transformB.scale.z) * 0.5f;

    float topA = transformA.position.y + colA.top * transformA.scale.y;
    float topB = transformB.position.y + colB.top * transformB.scale.y;
    float bottomA = transformA.position.y - colA.bottom * transformA.scale.y;
    float bottomB = transformB.position.y - colB.bottom * transformB.scale.y;

    if ((topA < bottomB) || (topB < bottomA))
        return collision;

    vec2 direction = posB2 - posA2;
    float dist = length(direction); 
    if (dist == 0.0f)
        return collision;

    direction = normalize(direction);
    float maxDist = colA.radius * pScaleA + colB.radius * pScaleB;

    if (dist < maxDist) {
        vec2 resolution = direction * (maxDist - dist);
        collision.resolution = vec3(resolution.x, 0.0f, resolution.y);
        collision.isColliding = true;
    }
    return collision;
}
