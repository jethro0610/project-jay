#include "MovementSystem.h"

void MovementSystem::Execute(
    Entity* entities,
    DesiredMovementComponent& desiredMovementComponent,
    GroundTraceComponent& groundTraceComponent,
    TransformComponent& transformComponent,
    VelocityComponent& velocityComponent,
    ColliderComponent& colliderComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];

        if (!entity.HasComponents
            <
            DesiredMovementComponent,
            GroundTraceComponent,
            TransformComponent,
            VelocityComponent,
            ColliderComponent
            > 
        ()) continue;

        vec3 velocity = velocityComponent.velocity[i];
        const float maxSpeed = desiredMovementComponent.maxSpeed[i];
        const bool onGround = groundTraceComponent.onGround[i];
        const vec3 desiredMovement = desiredMovementComponent.desiredMovement[i];
        const MoveMode moveMode = desiredMovementComponent.moveMode[i];

        if (onGround) {
            velocity.y = 0.0f;
            if (moveMode == MoveMode::Default) {
                velocity.x += desiredMovement.x * ACCELERATION;
                velocity.z += desiredMovement.z * ACCELERATION;
                velocity.x *= SPEED_DECAY;
                velocity.z *= SPEED_DECAY;
            }
            else if (moveMode == MoveMode::Ski) {
                velocity.x = desiredMovement.x * maxSpeed * 2.0f;
                velocity.z = desiredMovement.z * maxSpeed * 2.0f;
            }
        }
        else {
            velocity.y -= GRAVITY_ACCELERATION;
            velocity.y = -min(-velocity.y, MAX_GRAVITY);
        }

        // Apply the velocity
        transformComponent.transform[i].position_ += velocity * TIMESTEP;
        velocityComponent.velocity[i] = velocity;

        // Stick the entity to the ground after movement is executed
        if (onGround)
            transformComponent.transform[i].position_.y = groundTraceComponent.groundPosition[i].y + colliderComponent.radius[i];
    }

    // NOTE: Currently the velocity is stored as planar, so the normal of the surface isn't actually in the velocity.
    // To have slope slideoffs, it's necessary to get the velocity along the normal of the surface.
    // Maybe a projection would work?
}