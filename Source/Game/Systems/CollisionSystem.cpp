#include "CollisionSystem.h"

void CollisionSystem::Execute(
    World* world,
    Entity* entities,
    TransformComponent& transformComponent,
    ColliderComponent& colliderComponent,
    GroundTraceComponent& groundTraceComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.HasComponents<TransformComponent, ColliderComponent, GroundTraceComponent>())
            continue;

        float radius = colliderComponent.radius[i];

        // Raymarch towards the nearest point on the surface
        vec3 position = transformComponent.transform[i].position_;
        vec3 directionToSurface = -world->GetNormal(position, 2.0f);
        float distance = 0;
        for (int d = 0; d < 8; d++) {
            float distanceSample = world->GetDistance(position + directionToSurface * distance);
            if (distanceSample < 0.001f)
                break;

            distance += distanceSample;

            if (distance > radius) {
                distance = INFINITY;
                break;
            }
        }
        vec3 hitPosition = position + directionToSurface * distance;

        // Ignore collisions if they hit the bottom part of the sphere collider
        // and the entity is grounded. This ensures that the entity isn't
        // pushed by the surface and moves in a straight line.
        // The shape is similar to a half sphere
        bool hitGroundCutoff = false;
        if (groundTraceComponent.onGround[i]) {
            float groundCutoff = position.y - radius * GROUND_CUTOFF_SCALE;
            if (hitPosition.y < groundCutoff)
                hitGroundCutoff = true;
        }

        // Hit solving
        if (distance < radius && !hitGroundCutoff) {
            vec3 hitNormal = world->GetNormal(hitPosition);
            colliderComponent.hitNormal[i] = hitNormal;
            colliderComponent.hit[i] = true;
            transformComponent.transform[i].position_ += hitNormal * (radius - distance);
        }
        else {
            colliderComponent.hit[i] = false;
        }
    }
}