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
#include "../Game/Components/SkeletonComponent.h"
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

void PlayerController::Init(
    ParticleManager& particleManager, 
    TransformComponent& transformComponent
) {
    Transform& transform = transformComponent.renderTransform[PLAYER_ENTITY];
    dustEmitter_ = particleManager.RequestEmitter("p_dust");
    dustEmitter_->parent_ = &transform;
    cloudEmitter_ = particleManager.RequestEmitter("p_cloud");
    cloudEmitter_->parent_ = &transform;
    sparkEmitter_ = particleManager.RequestEmitter("p_spark");
    sparkEmitter_->parent_ = &transform;
}

void PlayerController::Execute(
    World& world, 
    SpreadManager& spreadManager, 
    Camera& camera,
    GroundTraceComponent& groundTraceComponent,
    MeterComponent& meterComponent,
    MovementComponent& movementComponent,
    SkeletonComponent& skeletonComponent,
    SpreadActivatorComponent& spreadActivatorComponent,
    TransformComponent& transformComponent,
    VelocityComponent& velocityComponent,
    Inputs inputs
) {
    dustEmitter_->active_ = movementComponent.speed[PLAYER_ENTITY] > 35;
    cloudEmitter_->velocityOffset_ = velocityComponent.velocity[PLAYER_ENTITY] * 0.75f;
    cloudEmitter_->active_ = false;
    sparkEmitter_->active_ = false;

    vec3& position = transformComponent.transform[PLAYER_ENTITY].position;
    quat cameraPlanarRotation = quat(vec3(0.0f, camera.lookX_, 0.0f));
    vec3 cameraPlanarForward = cameraPlanarRotation * Transform::worldForward;
    vec3 cameraPlanarRight = cameraPlanarRotation * Transform::worldRight;

    skeletonComponent.nextAnimationIndex[PLAYER_ENTITY] = 0;

    vec3 desiredMovement = cameraPlanarForward * inputs.forwardInput + cameraPlanarRight * inputs.sideInput;
    float moveLength = length(desiredMovement);
    if (moveLength < 0.1f)
        desiredMovement = vec3(0.0f);
    else
        skeletonComponent.nextAnimationIndex[PLAYER_ENTITY] = 1;

    if (moveLength >= 0.6f)
        desiredMovement = normalize(desiredMovement);

    movementComponent.desiredMovement[PLAYER_ENTITY] = desiredMovement;
    movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Default;
    spreadActivatorComponent.radius[PLAYER_ENTITY] = 0;
    spreadActivatorComponent.amount[PLAYER_ENTITY] = INT_MAX;

    if (movementComponent.speed[PLAYER_ENTITY] > 24.0f)
        skeletonComponent.nextAnimationIndex[PLAYER_ENTITY] = 1;

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
        skeletonComponent.nextAnimationIndex[PLAYER_ENTITY] = 3;
        cutCooldown_--;
    }
    else if (cutTimer_ >= TIME_TO_CUT){
        movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Flow;
        skeletonComponent.nextAnimationIndex[PLAYER_ENTITY] = 3;
        spreadActivatorComponent.radius[PLAYER_ENTITY] = -3;
        isDoingAction = true;
        actionMeter_ += 1;
    } 
    else if (inputs.flow && meterComponent.meter[PLAYER_ENTITY] > 0) {
        sparkEmitter_->active_ = true;
        movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Flow;
        skeletonComponent.nextAnimationIndex[PLAYER_ENTITY] = 3;
        spreadActivatorComponent.radius[PLAYER_ENTITY] = 1;
        isDoingAction = true;
        actionMeter_ += 2;
    }
    else if (inputs.ski && meterComponent.meter[PLAYER_ENTITY] > 0)  {
        cloudEmitter_->active_ = true;
        movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Ski;
        skeletonComponent.nextAnimationIndex[PLAYER_ENTITY] = 2;
        spreadActivatorComponent.radius[PLAYER_ENTITY] = 1;
    }

    if (!isDoingAction && actionMeter_ > 0)
        actionMeter_ = max(actionMeter_ - 6, 0);

    if (actionMeter_ >= MAX_ACTION_METER) {
        if (length(desiredMovement) > 0.0001f) {
            transformComponent.transform[PLAYER_ENTITY].rotation = quatLookAtRH(normalize(desiredMovement), Transform::worldUp);
            velocityComponent.velocity[PLAYER_ENTITY] = normalize(desiredMovement) * movementComponent.speed[PLAYER_ENTITY];
        }

        velocityComponent.velocity[PLAYER_ENTITY].y = 50.0f;
        actionMeter_ = 0;

        // spreadActivatorComponent.radius[PLAYER_ENTITY] = 12;
        // spreadActivatorComponent.amount[PLAYER_ENTITY] = 64;
    } 


    SCREENLINE(1, "Speed: " + std::to_string(movementComponent.speed[PLAYER_ENTITY]));
    SCREENLINE(2, "Meter: " + std::to_string(meterComponent.meter[PLAYER_ENTITY]));
    SCREENLINE(3, "Action: " + std::to_string(actionMeter_));
    SCREENLINE(4, "X-Vel: " + std::to_string(velocityComponent.velocity[PLAYER_ENTITY].x));
    SCREENLINE(5, "Y-Vel: " + std::to_string(velocityComponent.velocity[PLAYER_ENTITY].y));
    SCREENLINE(6, "Z-Vel: " + std::to_string(velocityComponent.velocity[PLAYER_ENTITY].z));
    SCREENLINE(7, "X: " + std::to_string(transformComponent.transform[PLAYER_ENTITY].position.x));
    SCREENLINE(8, "Y: " + std::to_string(transformComponent.transform[PLAYER_ENTITY].position.y));
    SCREENLINE(9, "Z: " + std::to_string(transformComponent.transform[PLAYER_ENTITY].position.z));
    SCREENLINE(10, "Cut Cooldown: " + std::to_string(cutCooldown_));
}
