#include "PlayerInputSystem.h"

void PlayerInputSystem::Execute(Inputs inputs, Camera* camera, Entity* entities, DesiredMovementComponent& desiredMovementComponent) {
    assert(desiredMovementComponent.recievesFrom[PLAYER_ENTITY] == RecieveMovementFrom::Player);

    quat cameraPlanarRotation = quat(vec3(0.0f, camera->lookX_, 0.0f));
    vec3 cameraPlanarForward = cameraPlanarRotation * Transform::worldForward;
    vec3 cameraPlanarRight = cameraPlanarRotation * Transform::worldRight;

    vec3 desiredMovement = cameraPlanarForward * inputs.forwardInput + cameraPlanarRight * inputs.sideInput;
    if (length(desiredMovement) > 1.0f)
        desiredMovement = normalize(desiredMovement);

    desiredMovementComponent.desiredMovement[PLAYER_ENTITY] = desiredMovement;

    if (inputs.ski) {
        desiredMovementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Ski;
    }
    else {
        desiredMovementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Default;
    }
}