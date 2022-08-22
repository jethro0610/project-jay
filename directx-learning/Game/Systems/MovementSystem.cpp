#include "MovementSystem.h"

void MovementSystem::Execute(
    float deltaTime,
    DesiredMovementComponents& desiredMovementComponents,
    GroundTraceComponents& groundTraceComponents,
    TransformComponents& transformComponents,
    VelocityComponents& velocityComponents,
    ColliderComponents& colliderComponents
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (desiredMovementComponents.recievesFrom[i] == RECIEVE_MOVEMENT_NONE)
            continue;

        bool onGround = groundTraceComponents.onGround[i];

        vec3 velocity = velocityComponents.velocity[i];
        velocity += desiredMovementComponents.desiredMovement[i] * 10.0f * deltaTime;

        // Apply the velocity
        transformComponents.transform[i].position_ += velocity * deltaTime;
        velocityComponents.velocity[i] = velocity;

        // Stick the entity to the ground
        if (onGround)
            transformComponents.transform[i].position_.y = groundTraceComponents.groundPosition[i].y + colliderComponents.radius[i];
    }

    // NOTE: Currently the velocity is stored as planar, so the normal of the surface isn't actually in the velocity.
    // To have slope slideoffs, it's necessary to get the velocity along the normal of the surface.
    // Maybe a projection would work?
}