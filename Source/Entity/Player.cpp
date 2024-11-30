#include "Player.h"
#include "Time/Time.h"
#include "Camera/Camera.h"
#include "Types/Inputs.h"
#include "Terrain/Terrain.h"
#include "Resource/ResourceManager.h"
#include "Particle/ParticleManager.h"
#include "Spread/SpreadManager.h"
#include "Logging/Logger.h"
#include "Logging/ScreenText.h"
#include "Time/Time.h"
#include "EntityList.h"
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/string_cast.hpp>
using namespace glm;

static constexpr int BAND = 0;
static constexpr int BODY = 1;
static constexpr int HAIR = 2;
static constexpr int MASK = 3;
static constexpr int PANTS = 4;
static constexpr int RIBBON = 5;
static constexpr int SHIRT = 6;
static constexpr int SLIPPERS = 7;

static constexpr float MIN_SPEED = 12.0f;
static constexpr float MAX_SPEED = 160.0f;
static constexpr float MIN_FRICTION = 0.015f;
static constexpr float MAX_FRICTION = 0.1f;
static constexpr float MOMENTUM_DECAY = 2.0f;
static constexpr float FRICTION_CAP = MAX_SPEED * 0.35f;
static constexpr float DEFAULT_ROTATION_SPEED = 0.25f;
static constexpr float SPIN_ROTATION_SPEED = 0.06f;
static constexpr float ITEM_ROTATION_SPEED = 0.1f;
static constexpr float SLOPE_ROTATION_SPEED = 0.02f;
static constexpr float ATTACK_ROTATION_SPEED = 0.025f;
static constexpr float JUMP_CHARGE_ROTATION_SPEED = 0.015f;
static constexpr float AIR_ROTATION_SPEED = 0.015f;
static constexpr float SLOPE_ACCELERATION = 4.0f;
static constexpr float SLOPE_DOWN_SCALING = 4.0f;
static constexpr float SLOPE_UP_SCALING = 1.5f;
static constexpr float MIN_SLOPE_ACCELERATION_SCALING = 0.15f;

static constexpr float MAX_JUMP_CHARGE = 30.0f;
static constexpr float MIN_JUMP_ADD = 25.0f;
static constexpr float MAX_JUMP_ADD = 60.0f;
static constexpr float MIN_JUMP_VEL_RATIO = 0.5f;
static constexpr float MAX_JUMP_VEL_RATIO = 1.0f;
static constexpr float DOWN_VEL_JUMP_MULT = 0.25f;
static constexpr float JUMP_CHARGE_SPEED = 1.0f;
static constexpr int MAX_COYOTE_TIME = 20;

static constexpr int MAX_CHARGE = 75;
static constexpr int STRONG_CHARGE_THRESH = 15;
static constexpr int ATTACK_STARTUP = 2;
static constexpr int ATTACK_ACTIVE = 13;
static constexpr int ATTACK_COOLDOWN = 10;
static constexpr int ATTACK_TIME = ATTACK_STARTUP + ATTACK_ACTIVE + ATTACK_COOLDOWN;

static constexpr float MIN_HOMING_VERTICAL_DELTA= 30;
static constexpr float MAX_HOMING_DISTANCE = 150;

EntityDependendies Player::GetStaticDependencies() {
    return {
        "sk_char",
        {
            "t_hair_m",
        }
    };
}

void Player::Init(Entity::InitArgs args) {
    moveMode_ = MM_Default;
    bonus_ = 0.0f;
    speed_ = MIN_SPEED;
    tilt_ = 0.0f;
    meter_ = 0.0f;
    charging_ = false;
    attackActiveTimer_ = ATTACK_TIME;
    attackCharge_ = 0;
    lastAttackCharge_ = 0;
    chargingJump_ = false;
    jumpCharge_ = 0;
    spinTime_ = 0;

    SetFlag(EF_SendPush, true);
    SetFlag(EF_RecievePush, true);
    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_StickToGround, true);
    SetFlag(EF_AlignToGround, true);
    SetFlag(EF_UseVelocity, true);
    SetFlag(EF_UseSkeleton, true);
    SetFlag(EF_Interpolate, true);
    SetFlag(EF_SendHits, true);
    SetFlag(EF_RecieveHits, true);
    SetFlag(EF_RecieveKnockback, true);
    SetFlag(EF_HurtFaceForward, true);
    SetFlag(EF_CaptureSeed, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("sk_char");
    skeleton_ = resourceManager.GetSkeleton("sk_char");

    for (int i = 0; i < 8; i++) {
        materials_[i].shadowShader = resourceManager.GetShader("vs_skeletal_s", "fs_depth_s");
        materials_[i].castShadows = true;
        materials_[i].properties = MaterialProperties::Default();
    }
    materials_[BAND].shader = resourceManager.GetShader("vs_skeletal", "fs_dfsa_color");
    materials_[BAND].properties.color = vec4(0.85f);

    materials_[BODY].shader = resourceManager.GetShader("vs_skeletal", "fs_dfsa_color");
    materials_[BODY].properties.color = vec4(0.95f);

    materials_[HAIR].shader = resourceManager.GetShader("vs_skeletal", "fs_dfsa_color_masked");
    materials_[HAIR].triangleType = TriangleType::TWO_SIDED;
    materials_[HAIR].properties.color = vec4(0.65f);
    materials_[HAIR].numTextures = 1;
    materials_[HAIR].textures[0] = resourceManager.GetTexture("t_hair_m");

    materials_[MASK].shader = resourceManager.GetShader("vs_skeletal", "fs_dfsa_color");
    materials_[MASK].properties.color = vec4(0.95f);

    materials_[PANTS].shader = resourceManager.GetShader("vs_skeletal", "fs_dfsa_color");
    materials_[PANTS].properties.color = vec4(0.75f);

    materials_[RIBBON].shader = resourceManager.GetShader("vs_skeletal", "fs_dfsa_color");
    materials_[RIBBON].properties.color = vec4(0.85f);

    materials_[SHIRT].shader = resourceManager.GetShader("vs_skeletal", "fs_dfsa_color");
    materials_[SHIRT].properties.color = vec4(0.9f, 0.8f, 0.65f, 1.0f);
    materials_[SHIRT].triangleType = TriangleType::TWO_SIDED;
    materials_[SHIRT].properties.specularBrightness = 0.0f;

    materials_[SLIPPERS].shader = resourceManager.GetShader("vs_skeletal", "fs_dfsa_color");
    materials_[SLIPPERS].properties.color = vec4(0.85f);

    pose_.resize(skeleton_->bones_.size());
    renderPose_.resize(skeleton_->bones_.size());

    ParticleManager& particleManager = args.particleManager;
    speedEmtter_ = particleManager.RequestEmitter();
    speedEmtter_->properties_.material.shader = resourceManager.GetShader("vs_particle_stretch", "fs_particle");
    speedEmtter_->properties_.material.transparencyType = TRANSPARENCY_ADDITIVE;
    speedEmtter_->properties_.material.castShadows = false;
    speedEmtter_->properties_.material.triangleType = TriangleType::TWO_SIDED;
    speedEmtter_->properties_.spawnInterval = 0.005f;
    speedEmtter_->properties_.spawnCount = 10;
    speedEmtter_->properties_.spawnRadius = 0.5f;
    speedEmtter_->properties_.lifetime = 0.1f;
    speedEmtter_->properties_.minScale = 0.15f;
    speedEmtter_->properties_.maxScale = 0.5f;
    speedEmtter_->properties_.endScale = 0.0f;
    speedEmtter_->properties_.minVelocity = vec4(-15.0f, 0.0f, 15.0f, 0.0f);
    speedEmtter_->properties_.maxVelocity = vec4(15.0f, 20.0f, 25.0f, 0.0f);
    speedEmtter_->properties_.startColor = vec4(1.0f, 1.0f, 1.0f, 0.75f);
    speedEmtter_->properties_.endColor = vec4(1.0f, 1.0f, 1.0f, 0.0f);

    spinEmitter_ = particleManager.RequestEmitter();
    spinEmitter_->properties_.material.shader = resourceManager.GetShader("vs_particle_stretch", "fs_particle");
    spinEmitter_->properties_.material.transparencyType = TRANSPARENCY_ADDITIVE;
    spinEmitter_->properties_.material.castShadows = false;
    spinEmitter_->properties_.material.triangleType = TriangleType::TWO_SIDED;
    spinEmitter_->properties_.localSpace = true;
    spinEmitter_->properties_.spawnInterval = 0.1f;
    spinEmitter_->properties_.spawnCount = 6;
    spinEmitter_->properties_.spawnRadius = 0.0f;
    spinEmitter_->properties_.lifetime = 0.5f;
    spinEmitter_->properties_.minScale = 0.1f;
    spinEmitter_->properties_.maxScale = 0.15f;
    spinEmitter_->properties_.endScale = 0.0f;
    spinEmitter_->properties_.minVelocity = vec4(-20.0f, 5.0f, -20.0f, 0.0f);
    spinEmitter_->properties_.maxVelocity = vec4(20.0f, 10.0f, 20.0f, 0.0f);
    spinEmitter_->properties_.acceleration.y = -25.0f;
    spinEmitter_->properties_.startColor = vec4(1.0f, 0.8f, 0.5f, 1.0f);
    spinEmitter_->properties_.endColor = vec4(1.0f, 0.9f, 0.65f, 0.0f);

    slopeEmitter_ = particleManager.RequestEmitter();
    slopeEmitter_->properties_.material.shader = resourceManager.GetShader("vs_particle", "fs_particle");
    slopeEmitter_->properties_.material.transparencyType = TRANSPARENCY_ADDITIVE;
    slopeEmitter_->properties_.material.castShadows = false;
    slopeEmitter_->properties_.spawnInterval = 0.1f;
    slopeEmitter_->properties_.spawnCount = 25;
    slopeEmitter_->properties_.spawnRadius = 1.0f;
    slopeEmitter_->properties_.lifetime = 0.5f;
    slopeEmitter_->properties_.minScale = 0.25f;
    slopeEmitter_->properties_.maxScale = 1.0f;
    slopeEmitter_->properties_.endScale = 0.0f;
    slopeEmitter_->properties_.minVelocity = vec4(-5.0f, 5.0f, 5.0f, 0.0f);
    slopeEmitter_->properties_.maxVelocity = vec4(5.0f, 5.0f, 15.0f, 0.0f);
    slopeEmitter_->properties_.acceleration.y = 1.0f;
    slopeEmitter_->properties_.startColor = vec4(1.0f, 1.0f, 1.0f, 0.75f);
    slopeEmitter_->properties_.endColor = vec4(1.0f, 1.0f, 1.0f, 0.0f);

    hitbox_.radius = 3.0f;
    hitbox_.top = 2.0f;
    hitbox_.bottom = 2.0f;
    hitbox_.active = false;

    hurtbox_.radius = 2.0f;
    hurtbox_.top = 1.0f;

    pushbox_.radius = 1.0f;
    pushbox_.top = 4.0f;
    overlapbox_.top = 2.0f;
    overlapbox_.radius = 1.0f;
    overlapbox_.bottom = 1.0f;
    #ifdef _DEBUG
    DBG_collider_ = pushbox_;
    #endif

    meter_ = 0.0f;
    item_ = Item::Boost;
    homingTarget_ = nullptr;
}

void Player::OnDestroy() {
    speedEmtter_->release_ = true;
    spinEmitter_->release_ = true;
    slopeEmitter_->release_ = true;
}

void Player::Update() {
    quat cameraPlanarRotation = quat(vec3(0.0f, camera_->lookX_, 0.0f));
    vec3 cameraPlanarForward = cameraPlanarRotation * Transform::worldForward;
    vec3 cameraPlanarRight = cameraPlanarRotation * Transform::worldRight;
    vec3 desiredMovement = cameraPlanarForward * inputs_->forwardInput + cameraPlanarRight * inputs_->sideInput;

    speedEmtter_->active_ = speed_ > 35.0f && onGround_;
    spinEmitter_->active_ = false;
    slopeEmitter_->active_ = false;

    if (inputs_->startJump && !charging_) {
        if (onGround_)
            chargingJump_= true; 
        else {
            float closestDistance = INFINITY;
            for (int i = 0; i < 128; i++) {
                Entity* entity = &((*entities_)[i]);
                if (entity == this)
                    continue;

                if (!entity->GetFlag(EF_Targetable))
                    continue;
                vec3 target = entity->GetTarget();

                float verticalDelta = transform_.position.y - target.y;
                if (verticalDelta < MIN_HOMING_VERTICAL_DELTA)
                    continue;

                vec3 planarPosition = vec3(transform_.position.x, 0.0f, transform_.position.z);
                vec3 entityPlanarPosition = vec3(target.x, 0.0f, target.z);
                vec3 planarCameraForward = camera_->transform_.GetForwardVector();
                planarCameraForward.y = 0.0f;
                planarCameraForward = normalize(planarCameraForward);

                float planarDist = distance(planarPosition, entityPlanarPosition);
                if (planarDist > MAX_HOMING_DISTANCE)
                    continue;

                vec3 planarVectorToEntity = normalize(entityPlanarPosition - planarPosition);
                if (dot(planarCameraForward, planarVectorToEntity) < 0.75f)
                    continue;

                if (planarDist < closestDistance) {
                    homingTarget_ = entity;
                    closestDistance = planarDist;
                }
            }
            planarVelocityBeforeHoming_ = length(vec3(velocity_.x, 0.0f, velocity_.z));
        }
    }

    if (onGround_)
        homingTarget_ = nullptr;

    if (homingTarget_ != nullptr) {
        vec3 vectorToTarget = normalize(homingTarget_->GetTarget() - transform_.position);
        velocity_ = vectorToTarget * 500.0f;
    }

    if (onGround_) 
        coyoteAirTime_ = 0;
    else if (coyoteAirTime_ <= MAX_COYOTE_TIME)
        coyoteAirTime_++;

    if (onGround_) {
        for (int i = 1; i < JUMP_BUFFER_FRAMES; i++)
            jumpBuffer_[i] = jumpBuffer_[i - 1]; 
        jumpBuffer_[0] = velocity_.y;
    }

    float greatestYVel = -INFINITY;
    for (int i = 0; i < JUMP_BUFFER_FRAMES; i++) {
        if (jumpBuffer_[i] > greatestYVel)
            greatestYVel = jumpBuffer_[i];
    }

    if (inputs_->releaseJump) {
        chargingJump_ = false;
        if (coyoteAirTime_ <= MAX_COYOTE_TIME) {
            float chargeRatio = std::lerp(MIN_JUMP_VEL_RATIO, MAX_JUMP_VEL_RATIO, jumpCharge_ / MAX_JUMP_CHARGE);
            chargeRatio = greatestYVel < 0.0f ? 1.0f - chargeRatio : chargeRatio;
            float jumpAdd = std::lerp(MIN_JUMP_ADD, MAX_JUMP_ADD, jumpCharge_ / MAX_JUMP_CHARGE);
            float jumpVelocity = greatestYVel * chargeRatio + jumpAdd;

            velocity_.y = max(jumpVelocity, jumpAdd);
            skipGroundCheck_ = true;
            jumpCharge_ = 0.0f;
        }
    }
    if (chargingJump_)
        jumpCharge_ += JUMP_CHARGE_SPEED;

    jumpCharge_ = min(jumpCharge_, MAX_JUMP_CHARGE);

    if (inputs_->useItem)
        UseItem();

    if (itemTimer_ > 0) {
        itemTimer_--;

        switch(item_) {
            case Item::None:
                break;

            case Item::Boost:
                break;

            case Item::Cut:
                spreadManager_->RemoveSpread(transform_.position, 6, nullptr);
                break;

            case Item::Radius:
                spreadManager_->AddSpread(transform_.position, 8);
                break;

            case Item::NumItems:
                break;
        }

        if (itemTimer_ == 0)
            item_ = Item::None;
    }

    if (itemMoveTimer_ > 0)
        itemMoveTimer_--;

    if (inputs_->startAttack && !chargingJump_)
        charging_ = true;
    if (inputs_->releaseAttack)
        charging_ = false;
    if (attackCharge_ > MAX_CHARGE)
        charging_ = false;

    if (charging_ && attackActiveTimer_ == ATTACK_TIME) {
        attackCharge_++;
    }
    else if (attackCharge_ != 0) {
        lastAttackCharge_ = attackCharge_;
        attackCharge_ = 0;
        attackActiveTimer_ = 0;
    }

    if (attackActiveTimer_ < ATTACK_TIME) {
        if (attackActiveTimer_ >= ATTACK_STARTUP && attackActiveTimer_ < ATTACK_STARTUP + ATTACK_ACTIVE) {
            hitbox_.active = true;
        }
        else 
            hitbox_.active = false;
        attackActiveTimer_++;
    }
    else
        hitbox_.active = false;

    moveMode_ = MM_Default;
    if (stun_) {
        moveMode_ = MM_Stun;
    }
    else if (homingTarget_ != nullptr) {
        moveMode_ = MM_Target;
    }
    else if (!onGround_) {
        moveMode_ = MM_Air;
    }
    else if (itemMoveTimer_ > 0) {
        moveMode_ = MM_Item;
    }
    else if (attackActiveTimer_ < ATTACK_TIME) {
        moveMode_ = MM_Attack;
    }
    else if (chargingJump_) {
        moveMode_ = MM_JumpCharge;
    }
    else if (attackCharge_ != 0) {
        moveMode_ = MM_Attack;
    }
    else if (inputs_->flow) {
        spinEmitter_->active_ = true;
        moveMode_ = MM_Spin;
    }
    else if (inputs_->ski)  {
        slopeEmitter_->active_ = true;
        moveMode_ = MM_Slope;
    }

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
    if (length(planarVelocity) > 0.0f)
        velocity_ -= normalize(planarVelocity) * bonus_;
    speed_ = length(planarVelocity) - bonus_;

    if (moveMode_ == MM_Spin)
        spinTime_ += 1;
    else
        spinTime_ -= 2;
    spinTime_ = clamp(spinTime_, 0, 120);

    if (homingTarget_ == nullptr)
        velocity_.y -= 2.0f;

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

        case MM_JumpCharge: {
            quat initialRotation = transform_.rotation;
            if (length(planarVelocity) > 0.1f)
                initialRotation = quatLookAtRH(normalize(planarVelocity), Transform::worldUp);

            quat desiredRotation = initialRotation;

            if (length(desiredMovement) > 0.001f) 
               desiredRotation = quatLookAtRH(normalize(desiredMovement), Transform::worldUp);

            transform_.rotation = slerp(transform_.rotation, desiredRotation, JUMP_CHARGE_ROTATION_SPEED);
            vec3 travelDirection = slerp(initialRotation, desiredRotation, JUMP_CHARGE_ROTATION_SPEED) * Transform::worldForward;
            
            velocity_.x = travelDirection.x * speed_;
            velocity_.z = travelDirection.z * speed_;
            break;
        }

        case MM_Spin: {
            quat rotation = transform_.rotation;
            if (length(desiredMovement) > 0.001f) 
                rotation = quatLookAtRH(normalize(desiredMovement), Transform::worldUp);
            transform_.rotation = slerp(transform_.rotation, rotation, SPIN_ROTATION_SPEED);
            vec3 direction = transform_.rotation * Transform::worldForward;
            
            if (spinTime_ >= 120)
                speed_ *= 0.99f;

            velocity_.x = direction.x * speed_;
            velocity_.z = direction.z * speed_;
            break;
        }

        case MM_Item: {
            quat rotation = transform_.rotation;
            if (length(desiredMovement) > 0.001f) 
                rotation = quatLookAtRH(normalize(desiredMovement), Transform::worldUp);
            transform_.rotation = slerp(transform_.rotation, rotation, ITEM_ROTATION_SPEED);
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
                dot(direction, vec3(groundNormal_.x, 0.0f, groundNormal_.z)) * 
                max(speed_ / MAX_SPEED, 0.01f);

            float multiplier = 1.0f;
            if (boost >= 0.0f) {
                multiplier = (MAX_SPEED - min(speed_, MAX_SPEED)) / MAX_SPEED;
                multiplier = max(multiplier, MIN_SLOPE_ACCELERATION_SCALING);
            }
            else
                multiplier = SLOPE_UP_SCALING;

            speed_ += boost * multiplier;
            speed_ = max(speed_, 0.0f);

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

        case MM_Air: {
            quat initialRotation = transform_.rotation;
            if (length(planarVelocity) > 0.1f)
                initialRotation = quatLookAtRH(normalize(planarVelocity), Transform::worldUp);

            quat desiredRotation = initialRotation;

            if (length(desiredMovement) > 0.001f) 
               desiredRotation = quatLookAtRH(normalize(desiredMovement), Transform::worldUp);

            transform_.rotation = slerp(transform_.rotation, desiredRotation, AIR_ROTATION_SPEED);
            vec3 travelDirection = slerp(initialRotation, desiredRotation, AIR_ROTATION_SPEED) * Transform::worldForward;
            
            velocity_.x = travelDirection.x * speed_;
            velocity_.z = travelDirection.z * speed_;
            break;
        }

        case MM_Stun:
            break;

        case MM_Target:
            break;
    }
    planarVelocity = vec3(velocity_.x, 0.0f, velocity_.z);
    bonus_ = bonus_ * 0.985f;
    if (length(planarVelocity) > 0.0f)
        velocity_ += normalize(planarVelocity) * bonus_;
    

    if (onGround_ && meter_ > 0 && itemTimer_ <= 0) {
        spreadRadius_ = lerp(spreadRadius_, 4.0f + meter_ * 12.0f, 0.05f);
        meter_ -= 0.0005f;
        meter_ = max(0.0f, meter_);
    }
    else {
        spreadRadius_ = lerp(spreadRadius_, 0.0f, 0.05f);
    }
    spreadManager_->AddSpread(transform_.position, int(spreadRadius_));

    if (lastAttackCharge_ < STRONG_CHARGE_THRESH) {
        hitbox_.knocback = planarVelocity * 0.975f;
        hitbox_.knocback.y = 35.0f;// + clamp(velocity_.y, -10.0f, 10.0f);
        hitbox_.hitlag = 4;
        hitbox_.diStrength = 0.5f;
    }
    else {
        hitbox_.knocback = planarVelocity * 1.5f;
        hitbox_.knocback.y = 35.0f;// + clamp(velocity_.y, -10.0f, 10.0f);
        hitbox_.hitlag = 6;
        hitbox_.diStrength = 0.25f;
    }

    if (onGround_ && stun_) {
        stun_ = false;
    }

    int animation = 0;
    float transitionLength = 0.35f;
    if (stun_) {
        animation = 7;
        transitionLength = 0.15f;
    }
    else if (attackActiveTimer_ < ATTACK_TIME) {
        animation = 4;
        transitionLength = 0.0f;
    }
    else if (attackCharge_ > 0) 
        animation = 5;
    else if (moveMode_ == MM_Spin || moveMode_ == MM_Item)
        animation = 3;
    else if (moveMode_ == MM_Slope)
        animation = 2;
    else if (moveLength > 0.0f)
        animation = 1;

    if (animation != animIndex_)
        ChangeAnimation(animation, transitionLength);

    float desiredTilt = 0.0f;
    if (!stun_) {
        if (moveLength > 0.001f)
            desiredTilt = dot(desiredMovement / moveLength, transform_.GetRightVector()) * 0.35f;
        tilt_ = lerp(tilt_, desiredTilt, 0.05f);
    }
    else
        tilt_ = 0.0f;

    if (!onGround_)
        traceDistance_ = std::max(-velocity_.y * GlobalTime::TIMESTEP, 0.0f);
    else
        traceDistance_ = 10.0f;

    SCREENLINE(1, std::to_string(speed_));
    SCREENLINE(2, std::to_string(jumpCharge_));
    SCREENLINE(3, std::to_string(spinTime_));
    SCREENLINE(4, std::to_string(bonus_));
    SCREENLINE(5, std::to_string(length(velocity_)));
    SCREENLINE(6, std::to_string(meter_));
    SCREENLINE(7, std::to_string(spreadRadius_));
}

void Player::RenderUpdate() {
    speedEmtter_->transform_ = renderTransform_;
    speedEmtter_->transform_.position += velocity_ * GlobalTime::TIMESTEP;
    spinEmitter_->transform_ = renderTransform_;
    slopeEmitter_->transform_ = renderTransform_;
    slopeEmitter_->transform_.position += velocity_ * GlobalTime::TIMESTEP;

    vec3 tiltUp = mix(Transform::worldUp, transform_.GetRightVector(), tilt_);
    tiltUp = normalize(tiltUp);
    quat tiltDelta = rotation(Transform::worldUp, tiltUp);
    renderTransform_.rotation = tiltDelta * renderTransform_.rotation;
}

void Player::OnHit(HitArgs args) {

}

void Player::OnHurt(HurtArgs args) {
    ChangeAnimation(6, 0.0f);
}

void Player::OnCaptureSeed() {
    meter_ += 0.0005f;
    meter_ = min(1.0f, meter_);
}

void Player::OnPush(vec3 pushVec) {
    vec3 planarPush = pushVec;
    planarPush.y = 0.0f;
    planarPush = normalize(planarPush);

    vec3 planarVelocity = velocity_;
    planarVelocity.y = 0.0f;
    float velocityLen = length(planarVelocity);
    planarVelocity = planarVelocity / velocityLen;

    if (attackActiveTimer_ != ATTACK_TIME) {
        velocity_ = -planarVelocity * velocityLen;
        transform_.rotation = quatLookAtRH(-planarVelocity, Transform::worldUp);
    }
    else if (speed_ >= 50.0f && dot(planarPush, planarVelocity) < -0.75f) {
        velocity_ = -planarVelocity * velocityLen * 0.5f;
        transform_.rotation = quatLookAtRH(-planarVelocity, Transform::worldUp);
        velocity_.y = 20.0f;
        stun_ = true;
        skipGroundCheck_ = true;
        onGround_ = false;
        ChangeAnimation(7, 0.0f);
    }
}

void Player::UseItem() {
    if (item_ == Item::None || itemTimer_ > 0)
        return;

    static constexpr int CUT_TIME = 4 * 60;
    static constexpr int CUT_MOVE_TIME = 4 * 60;

    static constexpr int RADIUS_TIME = 3 * 60;
    static constexpr int RADIUS_MOVE_TIME = 30;

    switch (item_) {
        case Item::None:
            break;

        case Item::NumItems:
            break;

        case Item::Boost: {
            velocity_ = transform_.GetForwardVector() * 60.0f;
            item_ = Item::None;
            break;
        }

        case Item::Cut: {
            itemTimer_ = CUT_TIME;
            itemMoveTimer_ = CUT_MOVE_TIME;
            break;
        }
        
        case Item::Radius: {
            itemTimer_ = RADIUS_TIME;
            itemMoveTimer_ = RADIUS_MOVE_TIME;
            break;
        }
    }
}

void Player::EndHoming() {
    if (homingTarget_ == nullptr) return;

    homingTarget_ = nullptr;
    vec3 planarDir = normalize(vec3(velocity_.x, 0.0f, velocity_.z));
    planarDir *= planarVelocityBeforeHoming_;
    velocity_ = vec3(planarDir.x, velocity_.y, planarDir.z);
}

void Player::Boost(float boost, float retention) {
    vec3 planarVelocity = vec3(velocity_.x, 0.0f, velocity_.z);
    velocity_ += normalize(planarVelocity) * boost;
    bonus_ = boost * (1.0f - retention);
}
