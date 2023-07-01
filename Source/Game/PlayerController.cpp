#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include "PlayerController.h"
#include "Camera.h"
#include "../Constants/GameConstants.h"
#include "World/SpreadManager.h"
#include "World/World.h"
#include "../Game/Components/GroundTraceComponent.h"
#include "../Game/Components/MeterComponent.h"
#include "../Game/Components/MovementComponent.h"
#include "../Game/Components/SpreadActivatorComponent.h"
#include "../Game/Components/TransformComponent.h"
#include "../Game/Components/VelocityComponent.h"
#include "../Logging/ScreenText.h"
using namespace glm;

PlayerController::PlayerController() {
    actionMeter_ = 0; 
    cutTimer_ = 0;
    cutCooldown_ = 0;
}

void PlayerController::Execute(
    World& world, 
    SpreadManager& spreadManager, 
    Camera& camera,
    GroundTraceComponent& groundTraceComponent,
    MeterComponent& meterComponent,
    MovementComponent& movementComponent,
    SpreadActivatorComponent& spreadActivatorComponent,
    TransformComponent& transformComponent,
    VelocityComponent& velocityComponent,
    Inputs inputs
) {
    vec3& position = transformComponent.transform[PLAYER_ENTITY].position_;
    quat cameraPlanarRotation = quat(vec3(0.0f, camera.lookX_, 0.0f));
    vec3 cameraPlanarForward = cameraPlanarRotation * Transform::worldForward;
    vec3 cameraPlanarRight = cameraPlanarRotation * Transform::worldRight;

    vec3 desiredMovement = cameraPlanarForward * inputs.forwardInput + cameraPlanarRight * inputs.sideInput;
    if (length(desiredMovement) >= 0.6f)
        desiredMovement = normalize(desiredMovement);

    movementComponent.desiredMovement[PLAYER_ENTITY] = desiredMovement;
    movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Default;
    spreadActivatorComponent.radius[PLAYER_ENTITY] = 0;
    spreadActivatorComponent.amount[PLAYER_ENTITY] = INT_MAX;

    if (inputs.flow && inputs.ski) {
        // Have to check this to prevent overflow
        if (cutTimer_ < TIME_TO_CUT)
            cutTimer_++;
    }
    else {
        if (cutTimer_ >= TIME_TO_CUT)
            cutCooldown_ = CUT_COOLDOWN_TIME;
        cutTimer_ = 0;
    }

    bool isDoingAction = false;
    if (!groundTraceComponent.onGround[PLAYER_ENTITY]) {
        movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Line;
    }
    else if (cutCooldown_ > 0) {
        movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Flow;
        cutCooldown_--;
    }
    else if (cutTimer_ >= TIME_TO_CUT){
        movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Flow;
        spreadActivatorComponent.radius[PLAYER_ENTITY] = -3;
        isDoingAction = true;
        actionMeter_ += 1;
    } 
    else if (inputs.flow && meterComponent.meter[PLAYER_ENTITY] > 0) {
        movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Flow;
        spreadActivatorComponent.radius[PLAYER_ENTITY] = 1;
        isDoingAction = true;
        actionMeter_ += 2;
    }
    else if (inputs.ski && meterComponent.meter[PLAYER_ENTITY] > 0)  {
        movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Ski;
        spreadActivatorComponent.radius[PLAYER_ENTITY] = 1;
    }

    if (!isDoingAction && actionMeter_ > 0)
        actionMeter_ = max(actionMeter_ - 3, 0);

    if (actionMeter_ >= MAX_ACTION_METER) {
        velocityComponent.velocity[PLAYER_ENTITY].y = 50.0f;
        actionMeter_ = 0;
        if (length(desiredMovement) > 0.0001f)
            transformComponent.transform[PLAYER_ENTITY].rotation_ = quatLookAtRH(normalize(desiredMovement), Transform::worldUp);

        spreadActivatorComponent.radius[PLAYER_ENTITY] = 6;
        spreadActivatorComponent.amount[PLAYER_ENTITY] = 32;
    } 

    SCREENLINE(0, "Speed: " + std::to_string(movementComponent.speed[PLAYER_ENTITY]));
    SCREENLINE(1, "Meter: " + std::to_string(meterComponent.meter[PLAYER_ENTITY]));
    SCREENLINE(2, "Action: " + std::to_string(actionMeter_));
}
