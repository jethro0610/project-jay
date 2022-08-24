#include "MovementSystem.h"

void MovementSystem::Execute(
    Entity* entities,
    DesiredMovementComponents& desiredMovementComponents,
    GroundTraceComponents& groundTraceComponents,
    TransformComponents& transformComponents,
    VelocityComponents& velocityComponents,
    ColliderComponents& colliderComponents
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.HasComponent<DesiredMovementComponents>())
            continue;
        if (!entity.HasComponent<GroundTraceComponents>())
            continue;
        if (!entity.HasComponent<TransformComponents>())
            continue;
        if (!entity.HasComponent<VelocityComponents>())
            continue;
        if (!entity.HasComponent<ColliderComponents>())
            continue;

        bool onGround = groundTraceComponents.onGround[i];
        vec3 velocity = velocityComponents.velocity[i];

        if (onGround) {
            velocity.y = 0.0f;
            velocity.x += desiredMovementComponents.desiredMovement[i].x * ACCELERATION;
            velocity.z += desiredMovementComponents.desiredMovement[i].z * ACCELERATION;
            velocity.x *= SPEED_DECAY;
            velocity.z *= SPEED_DECAY;
        }
        else {
            velocity.y -= GRAVITY_ACCELERATION;
            velocity.y = -min(-velocity.y, MAX_GRAVITY);
        }

        // Apply the velocity
        transformComponents.transform[i].position_ += velocity * TIMESTEP;
        velocityComponents.velocity[i] = velocity;

        // Stick the entity to the ground after movement is executed
        if (onGround)
            transformComponents.transform[i].position_.y = groundTraceComponents.groundPosition[i].y + colliderComponents.radius[i];
    }

    // NOTE: Currently the velocity is stored as planar, so the normal of the surface isn't actually in the velocity.
    // To have slope slideoffs, it's necessary to get the velocity along the normal of the surface.
    // Maybe a projection would work?
}