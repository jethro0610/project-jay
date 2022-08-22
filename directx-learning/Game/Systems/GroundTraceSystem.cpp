#include "GroundTraceSystem.h"

void GroundTraceSystem::Execute(
    World* world,
    TransformComponents& transformComponents,
    GroundTraceComponents& groundTraceComponents
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        float traceDistance = groundTraceComponents.distance[i];
        if (groundTraceComponents.distance[i] == NO_GROUND_TRACE)
            continue;

        groundTraceComponents.onGroundLastFrame[i] = groundTraceComponents.onGround[i];

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
    }
}