#include "PlayerController.h"

PlayerController::PlayerController(EntityManager& entityManager, Camera& camera) :
    entityManager_(entityManager),
    camera_(camera)
{
    actionMeter_ = 0.0f; 
}

void PlayerController::Execute(Inputs inputs) {
    MovementComponent& movementComponent = entityManager_.movementComponent_; 
    SpreadActivatorComponent& spreadActivatorComponent = entityManager_.spreadActivatorComponent_; 

    quat cameraPlanarRotation = quat(vec3(0.0f, camera_.lookX_, 0.0f));
    vec3 cameraPlanarForward = cameraPlanarRotation * Transform::worldForward;
    vec3 cameraPlanarRight = cameraPlanarRotation * Transform::worldRight;

    vec3 desiredMovement = cameraPlanarForward * inputs.forwardInput + cameraPlanarRight * inputs.sideInput;
    if (length(desiredMovement) >= 0.6f)
        desiredMovement = normalize(desiredMovement);

    movementComponent.desiredMovement[PLAYER_ENTITY] = desiredMovement;
    movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Default;
    spreadActivatorComponent.mode[PLAYER_ENTITY] = SpreadActivatorMode::NoSpread;

    if (inputs.flow && inputs.ski)
        cutTimer_++;
    else 
        cutTimer_ = 0;

    if (cutTimer_ > TIME_TO_CUT){
        movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Flow;
        spreadActivatorComponent.mode[PLAYER_ENTITY] = SpreadActivatorMode::Cut;
    } 
    else if (inputs.flow) {
        movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Flow;
        spreadActivatorComponent.mode[PLAYER_ENTITY] = SpreadActivatorMode::Spread;
    }
    else if (inputs.ski)  {
        movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Ski;
        spreadActivatorComponent.mode[PLAYER_ENTITY] = SpreadActivatorMode::Spread;
    }
}
