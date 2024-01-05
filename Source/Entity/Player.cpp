#include "Player.h"
#include "Time/Time.h"
#include "Camera/Camera.h"
#include "Types/Inputs.h"
#include "Terrain/Terrain.h"
#include "Resource/ResourceManager.h"
#include "Particle/ParticleManager.h"
#include "Logging/Logger.h"
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/string_cast.hpp>
using namespace glm;

void Player::Init(EntityS::InitArgs args)
{
    EntityS::Init(args);
    PlayerMoveMode moveMode_ = MM_Default;
    float speed_ = MIN_SPEED;

    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_StickToGround, true);
    SetFlag(EF_UseVelocity, true);
    SetFlag(EF_UseSkeleton, true);
    SetFlag(EF_Interpolate, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("sk_char");
    skeleton_ = resourceManager.GetSkeleton("sk_char");
    materials_[0] = resourceManager.GetMaterial("m_player");
    materials_[1] = resourceManager.GetMaterial("m_playerskin");
    materials_[2] = resourceManager.GetMaterial("m_hair");
    materials_[3] = resourceManager.GetMaterial("m_player");
    materials_[4] = resourceManager.GetMaterial("m_player");
    materials_[5] = resourceManager.GetMaterial("m_player");
    materials_[6] = resourceManager.GetMaterial("m_playershirt");
    materials_[7] = resourceManager.GetMaterial("m_player");

    pose_.resize(skeleton_->bones_.size());
    renderPose_.resize(skeleton_->bones_.size());

    ParticleManager& particleManager = args.particleManager;
    speedEmtter_ = particleManager.RequestEmitter(resourceManager.GetEmitterProperties("p_dust"));
    spinEmitter_ = particleManager.RequestEmitter(resourceManager.GetEmitterProperties("p_spark"));
    slopeEmitter_ = particleManager.RequestEmitter(resourceManager.GetEmitterProperties("p_cloud"));
}

void Player::Update() {
    speedEmtter_->active_ = false;
    spinEmitter_->active_ = false;
    slopeEmitter_->active_ = false;

    moveMode_ = MM_Default;
    if (inputs_->flow) {
        spinEmitter_->active_ = true;
        moveMode_ = MM_Spin;
    }
    else if (inputs_->ski)  {
        slopeEmitter_->active_ = true;
        moveMode_ = MM_Slope;
    }

    quat cameraPlanarRotation = quat(vec3(0.0f, camera_->lookX_, 0.0f));
    vec3 cameraPlanarForward = cameraPlanarRotation * Transform::worldForward;
    vec3 cameraPlanarRight = cameraPlanarRotation * Transform::worldRight;
    vec3 desiredMovement = cameraPlanarForward * inputs_->forwardInput + cameraPlanarRight * inputs_->sideInput;
    float moveLength = length(desiredMovement);
    if (moveLength < 0.1f) {
        moveLength = 0.0f;
        desiredMovement = vec3(0.0f);
    }
    else if (moveLength > 1.0f) {
        moveLength = 1.0f;
        desiredMovement = normalize(desiredMovement);
    }

    float frictionLerp = 1.0f - (min(speed_, FRICTION_CAP) - MIN_SPEED) / (FRICTION_CAP - MIN_SPEED);
    float friction = lerp(MIN_FRICTION, MAX_FRICTION, frictionLerp);
    float speedDecay = 1.0f - friction;
    float acceleration = ((speed_ / speedDecay) - speed_);

    vec3 planarVelocity = vec3(velocity_.x, 0.0f, velocity_.z);

    velocity_.y -= 1.0f;
    switch (moveMode_) {
        case MM_Default: {
            velocity_.x += desiredMovement.x * acceleration;
            velocity_.z += desiredMovement.z * acceleration;
            velocity_.x *= speedDecay;
            velocity_.z *= speedDecay;
            if (length(desiredMovement) > 0.001f) {
                quat rotation = quatLookAtRH(normalize(desiredMovement), Transform::worldUp);
                transform_.rotation = slerp(transform_.rotation, rotation, DEFAULT_ROTATION_SPEED);
            }

            speed_ -= MOMENTUM_DECAY;
            speed_ = max(speed_, MIN_SPEED);
            break;
        }

        case MM_Spin: {
            quat rotation = transform_.rotation;
            if (length(desiredMovement) > 0.001f) 
                rotation = quatLookAtRH(normalize(desiredMovement), Transform::worldUp);
            transform_.rotation = slerp(transform_.rotation, rotation, SPIN_ROTATION_SPEED);
            vec3 direction = transform_.rotation * Transform::worldForward;
            
            velocity_.x = direction.x * speed_;
            velocity_.z = direction.z * speed_;
            break;
        }

        case MM_Slope: {
            quat rotation;
            if (length(desiredMovement) > 0.001f) {
                rotation = quatLookAtRH(normalize(vec3(velocity_.x, 0.0f, velocity_.z)), Transform::worldUp);
                quat desiredSkiRotation = quatLookAtRH(normalize(desiredMovement), Transform::worldUp);
                rotation = slerp(rotation, desiredSkiRotation, SLOPE_ROTATION_SPEED);
                transform_.rotation = slerp(transform_.rotation, rotation, DEFAULT_ROTATION_SPEED);
            }
            else 
                rotation = transform_.rotation;

            vec3 direction = rotation * Transform::worldForward;
            float boost = 
                (SLOPE_ACCELERATION + (1 - groundNormal_.y) * SLOPE_DOWN_SCALING) *
                dot(direction, vec3(groundNormal_.x, 0.0f, groundNormal_.z)) * (speed_ / MAX_SPEED);
            float multiplier = 1.0f;
            if (boost >= 0.0f) {
                multiplier = (MAX_SPEED - min(speed_, MAX_SPEED)) / MAX_SPEED;
                multiplier = max(multiplier, MIN_SLOPE_ACCELERATION_SCALING);
            }
            else
                multiplier = SLOPE_UP_SCALING;

            speed_ += boost * multiplier;
            speed_ = clamp(speed_, 0.0f, MAX_SPEED);

            velocity_.x = direction.x * speed_;
            velocity_.z = direction.z * speed_;
            break;
        }

        case MM_Attack: {
            quat initialRotation = transform_.rotation;
            if (length(planarVelocity) > 0.1f)
                initialRotation = quatLookAtRH(normalize(planarVelocity), Transform::worldUp);

            quat desiredRotation = initialRotation;

            if (length(desiredMovement) > 0.001f) 
               desiredRotation = quatLookAtRH(normalize(desiredMovement), Transform::worldUp);

            transform_.rotation= slerp(transform_.rotation, desiredRotation, ATTACK_ROTATION_SPEED);
            vec3 travelDirection = slerp(initialRotation, desiredRotation, ATTACK_ROTATION_SPEED) * Transform::worldForward;
            
            velocity_.x = travelDirection.x * speed_;
            velocity_.z = travelDirection.z * speed_;
            break;
        }
    }

    int animation = 0;
    float transitionLength = 0.35f;
    if (moveLength > 0.0f)
        animation = 1;
    if (moveMode_ == MM_Spin)
        animation = 3;
    else if (moveMode_ == MM_Slope)
        animation = 2;

    if (animation != animIndex_)
        ChangeAnimation(animation, transitionLength_);
}

void Player::RenderUpdate() {
    speedEmtter_->transform_ = transform_;
    spinEmitter_->transform_ = transform_;
    slopeEmitter_->transform_ = transform_;
}
