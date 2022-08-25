#include "MovementSystem.h"

void MovementSystem::Execute(
    Entity* entities,
    MovementComponent& movementComponent,
    GroundTraceComponent& groundTraceComponent,
    TransformComponent& transformComponent,
    VelocityComponent& velocityComponent,
    ColliderComponent& colliderComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.HasComponents
            <
            MovementComponent,
            GroundTraceComponent,
            TransformComponent,
            VelocityComponent,
            ColliderComponent
            > 
        ()) continue;

        vec3 velocity = velocityComponent.velocity[i];
        float speed = movementComponent.speed[i];
        float friction = movementComponent.friction[i];
        quat rotation = transformComponent.transform[i].rotation_;

        const float minSpeed = movementComponent.minSpeed[i];
        const float maxSpeed = movementComponent.maxSpeed[i];
        const float minFriction = movementComponent.minFriction[i];
        const float maxFriction = movementComponent.maxFriction[i];
        const float momentumDecay = movementComponent.momentumDecay[i];
        const float speedDecay = 1.0f - friction;
        const bool onGround = groundTraceComponent.onGround[i];
        const vec3 desiredMovement = movementComponent.desiredMovement[i];
        const MoveMode moveMode = movementComponent.moveMode[i];
        const float acceleration = ((speed / speedDecay) - speed);

        speed = length(vec2(velocity.x, velocity.z));
        switch (moveMode) {
        case MoveMode::Default:
            CalculateDefaultMovement(desiredMovement, acceleration, speedDecay, momentumDecay, speed, velocity, rotation);
            break;

        case MoveMode::Ski:
            CalculateSkiMovement(desiredMovement, speed, velocity, rotation);
            break;

        default:
            break;
        }
        if (onGround)
            velocity.y = 0.0f;
        else {
            velocity.y -= GRAVITY_ACCELERATION;
            velocity.y = -min(-velocity.y, MAX_GRAVITY);
        }
        speed = clamp(speed, minSpeed, maxSpeed);
        const float frictionLerp = 1.0f - (speed - minSpeed) / (maxSpeed - minSpeed);
        friction = lerp(minFriction, maxFriction, frictionLerp);

        // Apply the velocity
        transformComponent.transform[i].position_ += velocity * TIMESTEP;
        transformComponent.transform[i].rotation_ = rotation;
        velocityComponent.velocity[i] = velocity;
        movementComponent.speed[i] = speed;
        movementComponent.friction[i] = friction;

        // Stick the entity to the ground after movement is executed
        // NOTE: This may make ground sticking inconsistent, maybe move this to its own system that runs around the end of the update
        if (onGround)
            transformComponent.transform[i].position_.y = groundTraceComponent.groundPosition[i].y + colliderComponent.radius[i];
    }

    // NOTE: Currently the velocity is stored as planar, so the normal of the surface isn't actually in the velocity.
    // To have slope slideoffs, it's necessary to get the velocity along the normal of the surface.
    // Maybe a projection would work?
}

void MovementSystem::CalculateDefaultMovement(
    const vec3& desiredMovement,
    const float& acceleration,
    const float& speedDecay,
    const float& momentumDecay,
    float& speed,
    vec3& velocity,
    quat& rotation
) {
    velocity.x += desiredMovement.x * acceleration;
    velocity.z += desiredMovement.z * acceleration;
    velocity.x *= speedDecay;
    velocity.z *= speedDecay;
    if (length(desiredMovement) > 0.001f) {
        quat desiredRotation = quatLookAtRH(normalize(desiredMovement), Transform::worldUp);
        rotation = slerp(rotation, desiredRotation, 0.25f);
    }
    speed -= momentumDecay;
}

void MovementSystem::CalculateSkiMovement(
    const vec3& desiredMovement, 
    float &speed, 
    vec3& velocity, 
    quat& rotation
) {
    speed += 0.15f;
    rotation = quatLookAtRH(normalize(velocity), Transform::worldUp);

    quat skiDirection;
    if (length(desiredMovement) > 0.001f) {
        skiDirection = quatLookAtRH(normalize(vec3(velocity.x, 0.0f, velocity.z)), Transform::worldUp);
        quat desiredSkiDirection = quatLookAtRH(normalize(desiredMovement), Transform::worldUp);
        skiDirection = slerp(skiDirection, desiredSkiDirection, 0.025f);
        rotation = skiDirection;
    }
    else {
        skiDirection = rotation;
    }
    vec3 skiVector = skiDirection * Transform::worldForward;

    velocity.x = skiVector.x * speed;
    velocity.z = skiVector.z * speed;
}