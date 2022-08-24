#include "GroundTraceSystem.h"

void GroundTraceSystem::Execute(
    World* world,
    Entity* entities,
    TransformComponents& transformComponents,
    GroundTraceComponents& groundTraceComponents
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entities[i].componentMask.test(TransformComponents::ID))
            continue;
        if (!entities[i].componentMask.test(GroundTraceComponents::ID))
            continue;

        groundTraceComponents.onGroundLastFrame[i] = groundTraceComponents.onGround[i];
        float traceDistance = groundTraceComponents.distance[i];

        // Raymarch towards the ground
        vec3 position = transformComponents.transform[i].position_;
        float distanceToSurface = 0;
        for (int d = 0; d < 8; d++) {
            float distanceSample = world->GetDistance(position - Transform::worldUp * distanceToSurface);
            if (distanceSample < 0.001f)
                break;

            distanceToSurface += distanceSample;

            if (distanceToSurface > traceDistance) {
                distanceToSurface = INFINITY;
                break;
            }
        }

        // Hit solving
        if (distanceToSurface < traceDistance) {
            groundTraceComponents.onGround[i] = true;
            vec3 groundPosition = position - Transform::worldUp * distanceToSurface;
            vec3 groundNormal = world->GetNormal(groundPosition);
            groundTraceComponents.groundPosition[i] = groundPosition;
            groundTraceComponents.groundNormal[i] = groundNormal;
        }
        else {
            groundTraceComponents.onGround[i] = false;
        }

        // Check if the entity entered/exited the ground on this frame
        if (!groundTraceComponents.onGroundLastFrame[i] && groundTraceComponents.onGround[i])
            groundTraceComponents.enteredGround[i] = true;
        else
            groundTraceComponents.enteredGround[i] = false;

        if (groundTraceComponents.onGroundLastFrame[i] && !groundTraceComponents.onGround[i])
            groundTraceComponents.exitedGround[i] = true;
        else
            groundTraceComponents.exitedGround[i] = false;
    }
}