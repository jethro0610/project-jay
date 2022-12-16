#include "GroundTraceSystem.h"
using namespace glm;

void GroundTraceSystem::Execute(
    World* world,
    Entity* entities,
    TransformComponent& transformComponent,
    GroundTraceComponent& groundTraceComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.HasComponents<TransformComponent, GroundTraceComponent>())
            continue;

        groundTraceComponent.onGroundLastFrame[i] = groundTraceComponent.onGround[i];
        float traceDistance = groundTraceComponent.distance[i];

        // Raymarch towards the ground
        vec3 position = transformComponent.transform[i].position_;
        float distanceToSurface = 0;
        for (int d = 0; d < 8; d++) {
            float distanceSample = world->GetDistance(position - Transform::worldUp * distanceToSurface);
            if (abs(distanceSample) < 0.001f)
                break;

            distanceToSurface += distanceSample;

            if (distanceToSurface > traceDistance) {
                distanceToSurface = INFINITY;
                break;
            }
        }

        // Hit solving
        if (distanceToSurface < traceDistance) {
            groundTraceComponent.onGround[i] = true;
            vec3 groundPosition = position - Transform::worldUp * distanceToSurface;
            vec3 groundNormal = world->GetNormal(groundPosition);
            groundTraceComponent.groundPosition[i] = groundPosition.y;
            groundTraceComponent.groundNormal[i] = groundNormal;
        }
        else {
            groundTraceComponent.onGround[i] = false;
        }

        // Check if the entity entered/exited the ground on this frame
        if (!groundTraceComponent.onGroundLastFrame[i] && groundTraceComponent.onGround[i])
            groundTraceComponent.enteredGround[i] = true;
        else
            groundTraceComponent.enteredGround[i] = false;

        if (groundTraceComponent.onGroundLastFrame[i] && !groundTraceComponent.onGround[i])
            groundTraceComponent.exitedGround[i] = true;
        else
            groundTraceComponent.exitedGround[i] = false;
    }
}
