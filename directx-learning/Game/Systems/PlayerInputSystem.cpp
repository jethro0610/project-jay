#include "PlayerInputSystem.h"

void PlayerInputSystem::Execute(Inputs inputs, Camera* camera, DesiredMovementComponents& desiredMovementComponents) {
    assert(desiredMovementComponents.recievesFrom[PLAYER_ENTITY] == RECIEVE_MOVEMENT_PLAYER);

    quat cameraPlanarRotation = quat(vec3(0.0f, camera->lookX_, 0.0f));
    vec3 cameraPlanarForward = cameraPlanarRotation * Transform::worldForward;
    vec3 cameraPlanarRight = cameraPlanarRotation * Transform::worldRight;

    vec3 desiredMovement = cameraPlanarForward * inputs.forwardInput + cameraPlanarRight * inputs.sideInput;
    if (length(desiredMovement) > 1.0f)
        desiredMovement = normalize(desiredMovement);

    desiredMovementComponents.desiredMovement[PLAYER_ENTITY] = desiredMovement;
}