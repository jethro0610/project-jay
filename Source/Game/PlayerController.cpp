#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include "PlayerController.h"
#include "Camera.h"
#include "../Constants/GameConstants.h"
#include "World/SpreadManager.h"
#include "World/World.h"
#include "../Game/Entity/Entity.h"
#include "../Game/Components/GroundTraceComponent.h"
#include "../Game/Components/HitboxComponent.h"
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

void PlayerController::Execute(
    EntityList& entities,
    ComponentList& components,
    World& world, 
    SpreadManager& spreadManager, 
    Camera& camera,
    Inputs inputs
) {
    const Entity& entity = entities[PLAYER_ENTITY];
    if (entities[PLAYER_ENTITY].stunTimer_ > 0)
        return;

    auto& groundTraceComponent = components.Get<GroundTraceComponent>();
    auto& hitboxComponent = components.Get<HitboxComponent>();
    auto& meterComponent = components.Get<MeterComponent>();
    auto& movementComponent = components.Get<MovementComponent>();
    auto& skeletonComponent = components.Get<SkeletonComponent>();
    auto& spreadActivatorComponent = components.Get<SpreadActivatorComponent>();
    auto& transformComponent = components.Get<TransformComponent>();
    auto& velocityComponent = components.Get<VelocityComponent>();

    entity.emitters_[DUST_EMITTER]->active_ = movementComponent.speed[PLAYER_ENTITY] > 35;
    entity.emitters_[CLOUD_EMITTER]->active_ = false;
    entity.emitters_[SPARK_EMITTER]->active_ = false;

    // TODO: Move this to a general system
    entity.emitters_[CLOUD_EMITTER]->velocityOffset_ = velocityComponent.velocity[PLAYER_ENTITY] * 0.75f;

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

    if (!inputs.attack)
        releasedCharge_ = true;

    if (inputs.attack && attackTimer_ <= 0 && releasedCharge_) {
        charge_++;
        attackHitbox_ = charge_ < STRONG_CHARGE ? 0 : 1;
    }

    if (!inputs.attack && charge_ > 0 || charge_ >= MAX_CHARGE) {
        charge_ = 0;
        attackTimer_ = ATTACK_TIME;
        releasedCharge_ = false;
    }

    for (Hitbox& hitbox : hitboxComponent.hitboxes[PLAYER_ENTITY])
        hitbox.active = false;

    float attackFrames = ATTACK_TIME - attackTimer_;
    if (attackFrames > ATTACK_ACTIVE_START && attackFrames < ATTACK_ACTIVE_END)
        hitboxComponent.hitboxes[PLAYER_ENTITY][attackHitbox_].active = true;

    if (attackTimer_ > 0)
        attackTimer_--;

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
    if (charge_ > 0) {
        movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Line;
        skeletonComponent.nextAnimationIndex[PLAYER_ENTITY] = 5;
    }
    else if (attackTimer_ > 0) {
        movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Line;
        skeletonComponent.nextAnimationIndex[PLAYER_ENTITY] = 4;
    }
    else if (!groundTraceComponent.onGround[PLAYER_ENTITY]) {
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
        entity.emitters_[SPARK_EMITTER]->active_ = true;
        movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Flow;
        skeletonComponent.nextAnimationIndex[PLAYER_ENTITY] = 3;
        spreadActivatorComponent.radius[PLAYER_ENTITY] = 2;
        isDoingAction = true;
        actionMeter_ += 2;
    }
    else if (inputs.ski && meterComponent.meter[PLAYER_ENTITY] > 0)  {
        entity.emitters_[CLOUD_EMITTER]->active_ = true;
        movementComponent.moveMode[PLAYER_ENTITY] = MoveMode::Ski;
        skeletonComponent.nextAnimationIndex[PLAYER_ENTITY] = 2;
        spreadActivatorComponent.radius[PLAYER_ENTITY] = 2;
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
    SCREENLINE(11, "Hitbox: " + std::to_string(attackHitbox_));
}
