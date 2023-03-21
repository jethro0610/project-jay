#include "PlayerInputSystem.h"
using namespace glm;

void PlayerInputSystem::Execute(
    EntityManager& entityManager,
    Camera& camera, 
    Inputs inputs, 
    MovementComponent& movementComponent,
    PickupComponent& pickupComponent,
    SpreadActivatorComponent& spreadActivatorComponent
) {
    quat cameraPlanarRotation = quat(vec3(0.0f, camera.lookX_, 0.0f));
    vec3 cameraPlanarForward = cameraPlanarRotation * Transform::worldForward;
    vec3 cameraPlanarRight = cameraPlanarRotation * Transform::worldRight;

    vec3 desiredMovement = cameraPlanarForward * inputs.forwardInput + cameraPlanarRight * inputs.sideInput;
    if (length(desiredMovement) >= 0.6f)
        desiredMovement = normalize(desiredMovement);

    movementComponent.desiredMovement[PLAYER_ENTITY] = desiredMovement;
    movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Default;
    spreadActivatorComponent.active[PLAYER_ENTITY] = false;
    pickupComponent.pickup[PLAYER_ENTITY] = false;
    if (inputs.pickup) {
        movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Flow;
        spreadActivatorComponent.active[PLAYER_ENTITY] = true;
        return;
    }
    if (inputs.ski)  {
        movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Ski;
        spreadActivatorComponent.active[PLAYER_ENTITY] = true;
    }
}
