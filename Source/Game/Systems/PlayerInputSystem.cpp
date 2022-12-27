#include "PlayerInputSystem.h"
using namespace glm;

void PlayerInputSystem::Execute(
    Inputs inputs, 
    Camera* camera, 
    Entity* entities, 
    MovementComponent& movementComponent,
    PickupComponent& pickupComponent,
    MeterComponent& meterComponent,
    SpreadActivatorComponent& spreadActivatorComponent
) {
    quat cameraPlanarRotation = quat(vec3(0.0f, camera->lookX_, 0.0f));
    vec3 cameraPlanarForward = cameraPlanarRotation * Transform::worldForward;
    vec3 cameraPlanarRight = cameraPlanarRotation * Transform::worldRight;

    vec3 desiredMovement = cameraPlanarForward * inputs.forwardInput + cameraPlanarRight * inputs.sideInput;
    if (length(desiredMovement) > 1.0f)
        desiredMovement = normalize(desiredMovement);

    movementComponent.desiredMovement[PLAYER_ENTITY] = desiredMovement;
    movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Default;
    spreadActivatorComponent.radius[PLAYER_ENTITY] = NO_SPREAD;
    pickupComponent.pickup[PLAYER_ENTITY] = false;
    if (inputs.pickup) {
        movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Flow;
        pickupComponent.pickup[PLAYER_ENTITY] = true;
        spreadActivatorComponent.radius[PLAYER_ENTITY] = 1.0f;
        return;
    }
    if (inputs.ski)  {
        movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Ski;
        spreadActivatorComponent.radius[PLAYER_ENTITY] = 1.0f;
    }
}
