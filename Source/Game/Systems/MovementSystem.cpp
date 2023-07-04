#include <glm/gtx/compatibility.hpp>
#include "MovementSystem.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityKey.h"
#include "../Components/GroundTraceComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/SpreadDetectComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../../Logging/Logger.h"
using namespace glm;

constexpr EntityKey key = GetEntityKey<
    GroundTraceComponent,
    MovementComponent,
    SpreadDetectComponent,
    TransformComponent,
    VelocityComponent
>();
constexpr EntityKey spreadKey = GetEntityKey<SpreadDetectComponent>();

void MovementSystem::Execute (
    Entity* entities,
    GroundTraceComponent& groundTraceComponent,
    MovementComponent& movementComponent,
    SpreadDetectComponent& spreadDetectComponent,
    TransformComponent& transformComponent,
    VelocityComponent& velocityComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;
        if (!entity.MatchesKey(key))
            continue;

        vec3& velocity = velocityComponent.velocity[i];
        float& speed = movementComponent.speed[i];
        float& friction = movementComponent.friction[i];
        quat& rotation = transformComponent.transform[i].rotation;

        const MoveMode moveMode = movementComponent.moveMode[i];
        const float minSpeed = movementComponent.minSpeed[i];
        const float maxSpeed = movementComponent.maxSpeed[i];
        const float minFriction = movementComponent.minFriction[i];
        const float maxFriction = movementComponent.maxFriction[i];
        const float frictionCapSpeed = maxSpeed * FRICTION_SPEED_CAP_PERCENT;
        const float momentumDecay = movementComponent.momentumDecay[i];
        const float speedDecay = 1.0f - friction;
        const bool onGround = groundTraceComponent.onGround[i];
        const vec3 desiredMovement = movementComponent.desiredMovement[i];
        const float acceleration = ((speed / speedDecay) - speed);
        const vec3 groundNormal = groundTraceComponent.groundNormal[i];

        float planarVelocitySize = length(vec2(velocity.x, velocity.z));
        speed = min(maxSpeed, planarVelocitySize);

        switch (moveMode) {
        case MoveMode::Default:
            CalculateDefaultMovement(desiredMovement, acceleration, speedDecay, momentumDecay, minSpeed, maxSpeed, speed, velocity, rotation);
            break;

        case MoveMode::Ski:
            CalculateSkiMovement(desiredMovement, groundNormal, minSpeed, maxSpeed, speed, velocity, rotation);
            break;

        case MoveMode::Flow:
            CalculateFlowMovement(desiredMovement, speed, velocity, rotation);
            break;

        case MoveMode::Line:
            CalculateLineMovement(desiredMovement, speed, velocity, rotation);
            break;

        default:
            break;
        }

        // Calculate gravity
        if (!onGround) {
            velocity.y -= GRAVITY_ACCELERATION;
            velocity.y = -min(-velocity.y, MAX_GRAVITY);
        }

        // Calculate friction 
        const float frictionLerp = 1.0f - (min(speed, frictionCapSpeed) - minSpeed) / (frictionCapSpeed - minSpeed);
        friction = lerp(minFriction, maxFriction, frictionLerp);

        // Spread contanct speed decline
        vec3 planarVelocity = vec3(velocity.x, 0.0f, velocity.z);
        float planarLength = length(planarVelocity);
        if (
            entity.MatchesKey(spreadKey) && 
            spreadDetectComponent.deteced[i] &&
            planarLength >= 0.0f &&
            onGround
        ) {
            planarVelocity *= 0.9975f; 
            // SUGGESTION: Maybe this can be per level? Maybe some mechanic to turn off decay? Loop back limit (1, 2, 3)?
            velocity.x = planarVelocity.x;
            velocity.z = planarVelocity.z;
        }
    }

    // NOTE: Currently the velocity is stored as planar, so the normal of the surface isn't actually in the velocity.
    // To have slope slideoffs, it's necessary to get the velocity along the normal of the surface.
    // Maybe a projection would work?

    // NOTE: One way to remedy instant speed cancelling is to rotate the velocity by the desired movement
    // then set the friction to a low value. Another may be to have state for stopping and pivoting, 
    // so the entity can carry its momentum in said state
}

void MovementSystem::CalculateDefaultMovement(
    const vec3& desiredMovement,
    const float& acceleration,
    const float& speedDecay,
    const float& momentumDecay,
    const float& minSpeed,
    const float& maxSpeed,
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
        rotation = slerp(rotation, desiredRotation, DEFAULT_ROTATION_SPEED);
    }

    speed -= momentumDecay;
    speed = max(speed, minSpeed);
}

void MovementSystem::CalculateSkiMovement(
    const vec3& desiredMovement, 
    const vec3& groundNormal,
    const float& minSpeed,
    const float& maxSpeed,
    float &speed, 
    vec3& velocity, 
    quat& rotation
) {
    quat skiRotation;
    if (length(desiredMovement) > 0.001f) {
        skiRotation = quatLookAtRH(normalize(vec3(velocity.x, 0.0f, velocity.z)), Transform::worldUp);
        quat desiredSkiRotation = quatLookAtRH(normalize(desiredMovement), Transform::worldUp);
        skiRotation = slerp(skiRotation, desiredSkiRotation, SKI_ROTATION_SPEED);
        rotation = slerp(rotation, skiRotation, DEFAULT_ROTATION_SPEED);
    }
    else 
        skiRotation = rotation;

    vec3 skiDirection = skiRotation * Transform::worldForward;
    float skiBoost = 
        (SKI_ACCELERATION + (1 - groundNormal.y) * SKI_DOWNSLOPE_SCALING) *
        dot(skiDirection, vec3(groundNormal.x, 0.0f, groundNormal.z)) * (speed / maxSpeed);
    float skiMultiplier = 1.0f;
    if (skiBoost >= 0.0f) {
        skiMultiplier = (maxSpeed - min(speed, maxSpeed)) / maxSpeed;
        skiMultiplier = max(skiMultiplier, MINIMUM_SKI_ACCELERATION_SCALING);
    }
    else
        skiMultiplier = SKI_UPSLOPE_SCALING;

    speed += skiBoost * skiMultiplier;
    speed = clamp(speed, 0.0f, maxSpeed);

    velocity.x = skiDirection.x * speed;
    velocity.z = skiDirection.z * speed;
}

void MovementSystem::CalculateFlowMovement(
    const glm::vec3& desiredMovement,
    const float& speed,
    glm::vec3& velocity,
    glm::quat& rotation
) {
    quat desiredRotation = rotation;
    if (length(desiredMovement) > 0.001f) 
        desiredRotation = quatLookAtRH(normalize(desiredMovement), Transform::worldUp);
    rotation = slerp(rotation, desiredRotation, FLOW_ROTATION_SPEED);
    vec3 skiDirection = rotation * Transform::worldForward;
    
    velocity.x = skiDirection.x * speed;
    velocity.z = skiDirection.z * speed;
}

void MovementSystem::CalculateLineMovement(
    const glm::vec3& desiredMovement,
    const float& speed,
    glm::vec3& velocity,
    glm::quat& rotation
) {
    quat rotationToUse = rotation;
    vec3 planarVelocity = vec3(velocity.x, 0.0f, velocity.z);
    if (length(planarVelocity) > 0.1f)
        rotationToUse = quatLookAtRH(normalize(planarVelocity), Transform::worldUp);

    quat desiredRotation = rotationToUse;

    if (length(desiredMovement) > 0.001f) 
        desiredRotation = quatLookAtRH(normalize(desiredMovement), Transform::worldUp);

    rotation = slerp(rotation, desiredRotation, LINE_ROTATION_SPEED);
    rotationToUse = slerp(rotationToUse, desiredRotation, LINE_ROTATION_SPEED);
    vec3 lineDirection = rotationToUse * Transform::worldForward;
    
    velocity.x = lineDirection.x * speed;
    velocity.z = lineDirection.z * speed;
}
