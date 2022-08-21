#include "CollisionSystem.h"

void CollisionSystem::Execute(
    World* world,
    ActiveEntityComponents& activeComponents,
    TransformComponents& transformComponents,
    ColliderComponents& colliderComponents
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!activeComponents.active[i])
            continue;

        float radius = colliderComponents.radius[i];
        if (colliderComponents.radius[i] == -1.0f)
            continue;

        vec3 position = transformComponents.transform[i].position_;
        float distance = world->GetDistance(transformComponents.transform[i].position_);
        if (distance <= radius) {
            vec3 hitNormal = world->GetNormal(position, 2.0f);
            colliderComponents.hitNormal[i] = hitNormal;
            transformComponents.transform[i].position_ -= hitNormal * distance;
        }
        else
            colliderComponents.hitNormal[i] = NO_HIT_NORMAL;
    }
}