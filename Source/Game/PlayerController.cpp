#include "PlayerController.h"
#include "../Logging/ScreenText.h"

PlayerController::PlayerController(EntityManager& entityManager, SpreadManager& spreadManager, Camera& camera) :
    entityManager_(entityManager),
    spreadManager_(spreadManager),
    camera_(camera)
{
    actionMeter_ = 0; 
}

void PlayerController::Execute(Inputs inputs) {
    MovementComponent& movementComponent = entityManager_.movementComponent_; 
    TransformComponent& transformComponent = entityManager_.transformComponent_;
    VelocityComponent& velocityComponent = entityManager_.velocityComponent_;
    SpreadActivatorComponent& spreadActivatorComponent = entityManager_.spreadActivatorComponent_; 
    GroundTraceComponent& groundTraceComponent = entityManager_.groundTraceComponent_;

    quat cameraPlanarRotation = quat(vec3(0.0f, camera_.lookX_, 0.0f));
    vec3 cameraPlanarForward = cameraPlanarRotation * Transform::worldForward;
    vec3 cameraPlanarRight = cameraPlanarRotation * Transform::worldRight;

    vec3 desiredMovement = cameraPlanarForward * inputs.forwardInput + cameraPlanarRight * inputs.sideInput;
    if (length(desiredMovement) >= 0.6f)
        desiredMovement = normalize(desiredMovement);

    movementComponent.desiredMovement[PLAYER_ENTITY] = desiredMovement;
    movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Default;
    spreadActivatorComponent.radius[PLAYER_ENTITY] = 0;

    if (inputs.flow && inputs.ski) {
        // Have to check this to prevent overflow
        if (cutTimer_ < TIME_TO_CUT)
            cutTimer_++;
    }
    else 
        cutTimer_ = 0;

    bool isDoingAction = false;
    if (!groundTraceComponent.onGround[PLAYER_ENTITY]) {
        movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Line;
    }
    else if (cutTimer_ >= TIME_TO_CUT){
        movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Flow;
        spreadActivatorComponent.radius[PLAYER_ENTITY] = -3;
        isDoingAction = true;
        actionMeter_ += 1;
    } 
    else if (inputs.flow) {
        movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Flow;
        spreadActivatorComponent.radius[PLAYER_ENTITY] = 1;
        isDoingAction = true;
        actionMeter_ += 2;
    }
    else if (inputs.ski)  {
        movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Ski;
        spreadActivatorComponent.radius[PLAYER_ENTITY] = 1;
    }


    if (!isDoingAction && actionMeter_ > 0)
        actionMeter_ = max(actionMeter_ - 3, 0);

    if (actionMeter_ >= MAX_ACTION_METER) {
        // entityManager_.velocityComponent_.velocity[PLAYER_ENTITY] *= 0.75f;  
        entityManager_.velocityComponent_.velocity[PLAYER_ENTITY].y = 50.0f;
        spreadManager_.AddSpread(transformComponent.transform[PLAYER_ENTITY].position_, 6);
        // entityManager_.transformComponent_.transform->position_.y += 20.0f;
        actionMeter_ = 0;
    } 

    SCREENLINE(0, "Speed: " + std::to_string(movementComponent.speed[PLAYER_ENTITY]));
    SCREENLINE(1, "Action: " + std::to_string(actionMeter_));
    SCREENLINE(2, "Y-Vel: " + std::to_string(velocityComponent.velocity[PLAYER_ENTITY].y));
}
