#include "Player.h"
#include "Particle/ParticleEmitter.h"
#include "Camera/Camera.h"
#include "Collision/Hurtbox.h"
#include "Collision/Hitbox.h"
#include "Logging/ScreenText.h"
#include <glm/gtx/compatibility.hpp>
using namespace glm;

Player::Player(
    Camera& camera, 
    Inputs& inputs
):
camera_(camera),
inputs_(inputs)
{
    
}

void Player::Update() {
    quat cameraPlanarRotation = quat(vec3(0.0f, camera_.lookX_, 0.0f));
    vec3 cameraPlanarForward = cameraPlanarRotation * Transform::worldForward;
    vec3 cameraPlanarRight = cameraPlanarRotation * Transform::worldRight;
    desiredMovement_ = cameraPlanarForward * inputs_.forwardInput + cameraPlanarRight * inputs_.sideInput;

    speedEmitter_->active_ = speed_ > 35;// && groundTraceComponent.onGround[PLAYER_ENTITY];
    slopeEmitter_->active_ = false;
    slopeEmitter_->velocityOffset_ = velocity_;
    spinEmitter_->active_ = false;

    animationController_.SetAnimationIndex(0, 0.35f);

    float moveLength = length(desiredMovement_);
    if (moveLength < 0.1f)
        desiredMovement_ = vec3(0.0f);
    if (moveLength >= 0.6f)
        desiredMovement_ = normalize(desiredMovement_);

    if (hurtbox_->hurt) {
        attackCharge_ = 0;
        attackActiveTimer_ = 0;
        stun_ = true;
        // Apply knockback
        return;
    }
    else if (stun_) {
        return;
    }

    if (inputs_.chargeAttack && attackActiveTimer_ <= 0)
        attackCharge_++;

    if (inputs_.releaseAttack || attackCharge_ >= MAX_ATTACK_CHARGE) {
        if (attackCharge_ >= STRONG_ATTACK_CHARGE)
            currentAttackIndex_= STRONG_ATTACK_INDEX;
        else
            currentAttackIndex_ = WEAK_ATTACK_INDEX;

        attackCharge_ = 0;
        attackActiveTimer_ = TOTAL_ATTACK_TIME;
    }

    for (int i = 0; i < 2; i++)
        hitboxes_[i]->active = false;

    float attackFrames = TOTAL_ATTACK_TIME - attackActiveTimer_;
    if (attackFrames > ATTACK_STARTUP && attackFrames < ATTACK_STARTUP + ATTACK_ACTIVE)
        hitboxes_[currentAttackIndex_]->active = true;

    if (attackActiveTimer_ > 0)
        attackActiveTimer_--;

    moveMode_ = PM_Normal;
    if (attackCharge_ > 0) {
        moveMode_ = PM_Attack;
    }
    else if (attackActiveTimer_ > 0) {
        moveMode_ = PM_Attack;
    }
    else if (inputs_.flow) {
        moveMode_ = PM_Spin;
        spinEmitter_->active_ = true;
        // spreadActivatorComponent.radius[PLAYER_ENTITY] = 2;
    }
    else if (inputs_.ski)  {
        moveMode_ = PM_Slope;
        slopeEmitter_->active_ = true;
        // spreadActivatorComponent.radius[PLAYER_ENTITY] = 2;
    }

    SCREENLINE(1, "Speed: " + std::to_string(speed_));
    SCREENLINE(2, "X-Vel: " + std::to_string(velocity_.x));
    SCREENLINE(3, "Y-Vel: " + std::to_string(velocity_.y));
    SCREENLINE(4, "Z-Vel: " + std::to_string(velocity_.z));
    SCREENLINE(5, "X: " + std::to_string(transform_.position.x));
    SCREENLINE(6, "X: " + std::to_string(transform_.position.x));
    SCREENLINE(7, "X: " + std::to_string(transform_.position.x));

    Character::Update();
}

void Player::SetAnimation() {
    int animationIndex = 0;
    float transitionTime = 0.35f;

    if (hurtbox_->hurt) {
        animationIndex = 6;
        transitionTime = 0.0f;
    }
    else if (stun_) {
        animationIndex = 7;
        transitionTime = 0.15f;
    }
    else if (attackCharge_ > 0)
        animationIndex = 5;
    else if (attackActiveTimer_ > 0) {
        animationIndex = 4;
        transitionTime = 0.0f;
    }
    else if (moveMode_ == PM_Spin)
        animationIndex = 3;
    else if (moveMode_ == PM_Slope)
        animationIndex = 2;
    else if (moveMode_ == PM_Normal && length(desiredMovement_) > 0.0f)
        animationIndex = 1;

    animationController_.SetAnimationIndex(animationIndex);
}

void MoveNormal(
    const vec3& desiredMovement,
    const float& acceleration,
    const float& speedDecay,
    vec3& velocity,
    quat& rotation
) {
    velocity.x += desiredMovement.x * acceleration;
    velocity.z += desiredMovement.z * acceleration;

    velocity.x *= speedDecay;
    velocity.z *= speedDecay;

    if (length(desiredMovement) > 0.001f) {
        quat desiredRotation = quatLookAtRH(normalize(desiredMovement), Transform::worldUp);
        rotation = slerp(rotation, desiredRotation, Player::ROTATION_SPEED);
    }
}

void MoveSlope(
    const vec3& desiredMovement, 
    const vec3& groundNormal,
    float& speed, 
    vec3& velocity, 
    quat& rotation
) {
    quat slopeRotation;
    if (length(desiredMovement) > 0.001f) {
        slopeRotation = quatLookAtRH(normalize(vec3(velocity.x, 0.0f, velocity.z)), Transform::worldUp);
        quat desiredSkiRotation = quatLookAtRH(normalize(desiredMovement), Transform::worldUp);
        slopeRotation = slerp(slopeRotation, desiredSkiRotation, Player::SLOPE_ROTATION_SPEED);
        rotation = slerp(rotation, slopeRotation, Player::ROTATION_SPEED);
    }
    else 
        slopeRotation = rotation;

    vec3 slopeDirection = slopeRotation * Transform::worldForward;
    float slopeBoost = 
        (Player::SLOPE_ACCELERATION + (1 - groundNormal.y) * Player::SLOPE_DOWN_SCALING) *
        dot(slopeDirection, vec3(groundNormal.x, 0.0f, groundNormal.z)) * (speed / Player::MAX_SPEED);
    float slopeMultiplier = 1.0f;
    if (slopeBoost >= 0.0f) {
        slopeMultiplier = (Player::MAX_SPEED - min(speed, Player::MAX_SPEED)) / Player::MAX_SPEED;
        slopeMultiplier = max(slopeMultiplier, Player::MINIMUM_SLOPE_ACCELERATION_SCALING);
    }
    else
        slopeMultiplier = Player::SLOPE_UP_SCALING;

    speed += slopeBoost * slopeMultiplier;
    speed = clamp(speed, 0.0f, Player::MAX_SPEED);

    velocity.x = slopeDirection.x * speed;
    velocity.z = slopeDirection.z * speed;
}

void MoveLine(
    const glm::vec3& desiredMovement,
    const float& speed,
    glm::vec3& velocity,
    glm::quat& rotation,
    float rotationSpeed
) {
    quat desiredRotation = rotation;
    if (length(desiredMovement) > 0.001f) 
        desiredRotation = quatLookAtRH(normalize(desiredMovement), Transform::worldUp);
    rotation = slerp(rotation, desiredRotation, rotationSpeed);
    vec3 skiDirection = rotation * Transform::worldForward;
    
    velocity.x = skiDirection.x * speed;
    velocity.z = skiDirection.z * speed;
}

void Player::CalculateVelocity() {
    velocity_.y -= 1.0f;
    velocity_.y = -min(-velocity_.y, 120.0f);

    const float frictionLerp = 1.0f - (min(speed_, FRICTION_CAP_SPEED) - MIN_SPEED) / (FRICTION_CAP_SPEED - MIN_SPEED);
    const float friction = lerp(MIN_FRICTION, MAX_FRICTION, frictionLerp);
    const float speedDecay = 1.0f - friction;
    const float acceleration = ((speed_ / speedDecay) - speed_);

    float planarVelocitySize = length(vec2(velocity_.x, velocity_.z));
    speed_ = min(MAX_SPEED, planarVelocitySize);

    switch (moveMode_) {
        case PM_Normal:
            MoveNormal(desiredMovement_, acceleration, speedDecay, velocity_, transform_.rotation);
            break;

        case PM_Slope:
            MoveSlope(desiredMovement_, groundNormal_, speed_, velocity_, transform_.rotation); 
            break;

        case PM_Spin:
            MoveLine(desiredMovement_, speed_, velocity_, transform_.rotation, SPIN_ROTATION_SPEED);
            break;

        case PM_Attack:
            MoveLine(desiredMovement_, speed_, velocity_, transform_.rotation, ATTACK_ROTATION_SPEED);
            break;
    }

    speed_ -= MOMENTUM_DECAY;
    speed_ = max(speed_, MIN_SPEED);
}
