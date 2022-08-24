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
        float speed = desiredMovementComponent.speed[i];
        float friction = desiredMovementComponent.friction[i];

        const float minSpeed = desiredMovementComponent.minSpeed[i];
        const float maxSpeed = desiredMovementComponent.maxSpeed[i];
        const float minFriction = desiredMovementComponent.minFriction[i];
        const float maxFriction = desiredMovementComponent.maxFriction[i];
        const float momentumDecay = desiredMovementComponent.momentumDecay[i];
        const float speedDecay = 1.0f - friction;
        const bool onGround = groundTraceComponent.onGround[i];
        const vec3 desiredMovement = desiredMovementComponent.desiredMovement[i];
        const MoveMode moveMode = desiredMovementComponent.moveMode[i];
        const float acceleration = ((speed / speedDecay) - speed);

        if (onGround) {
            velocity.y = 0.0f;
            if (moveMode == MoveMode::Default) {
                velocity.x += desiredMovement.x * acceleration;
                velocity.z += desiredMovement.z * acceleration;
                velocity.x *= speedDecay;
                velocity.z *= speedDecay;
            }
            else if (moveMode == MoveMode::Ski) {
                speed += 0.15f;
                speed = min(speed, maxSpeed);

                velocity.x = desiredMovement.x * speed;
                velocity.z = desiredMovement.z * speed;
            }
        }
        else {
            velocity.y -= GRAVITY_ACCELERATION;
            velocity.y = -min(-velocity.y, MAX_GRAVITY);
        }

        if (moveMode == MoveMode::Default) {
            speed -= momentumDecay;
            speed = max(speed, minSpeed);
        }
        const float frictionLerp = 1.0f - (speed - minSpeed) / (maxSpeed - minSpeed);
        friction = lerp(minFriction, maxFriction, frictionLerp);


        // Apply the velocity
        transformComponent.transform[i].position_ += velocity * TIMESTEP;
        velocityComponent.velocity[i] = velocity;
        desiredMovementComponent.speed[i] = speed;
        desiredMovementComponent.friction[i] = friction;

        // Stick the entity to the ground after movement is executed
        // NOTE: This may make ground sticking inconsistent, maybe move this to its own system that runs around the end of the update
        if (onGround)
            transformComponent.transform[i].position_.y = groundTraceComponent.groundPosition[i].y + colliderComponent.radius[i];
    }

    // NOTE: Currently the velocity is stored as planar, so the normal of the surface isn't actually in the velocity.
    // To have slope slideoffs, it's necessary to get the velocity along the normal of the surface.
    // Maybe a projection would work?
}