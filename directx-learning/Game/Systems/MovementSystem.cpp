#include "MovementSystem.h"

void MovementSystem::Execute(
    float deltaTime,
    DesiredMovementComponents& desiredMovementComponents,
    GroundTraceComponents& groundTraceComponents,
    TransformComponents& transformComponents,
    VelocityComponents& velocityComponents
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (desiredMovementComponents.recievesFrom[i] == RECIEVE_MOVEMENT_NONE)
            continue;

        vec3 velocity = velocityComponents.velocity[i];
        velocity += desiredMovementComponents.desiredMovement[i] * 10.0f * deltaTime;

        transformComponents.transform[i].position_ += velocity * deltaTime;
        velocityComponents.velocity[i] = velocity;
    }
}