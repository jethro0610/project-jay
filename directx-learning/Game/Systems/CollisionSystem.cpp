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

        // Move downwards for testing
        transformComponents.transform[i].position_.y -= 0.1f;

        // Raymarch towards the nearest point on the surface
        vec3 position = transformComponents.transform[i].position_;
        vec3 directionToSurface = -world->GetNormal(position, 0.005f);
        float distance = 0;
        for (int d = 0; d < 8; d++) {
            float distanceSample = world->GetDistance(position + directionToSurface * distance);
            if (distanceSample < 0.0001f)
                break;

            distance += distanceSample;

            if (distance > radius) {
                distance = INFINITY;
                break;
            }
        }

        // Hit solving
        if (distance < radius) {
            vec3 hitNormal = world->GetNormal(position + directionToSurface * distance);
            colliderComponents.hitNormal[i] = hitNormal;
            transformComponents.transform[i].position_ += hitNormal * (radius - distance);
        }
        else {
            colliderComponents.hitNormal[i] = NO_HIT_NORMAL;
        }
    }
}