#include "Player.h"
#include "Time/Time.h"
#include "Camera/Camera.h"
#include "Types/Inputs.h"
#include "Terrain/Terrain.h"
#include "Resource/ResourceManager.h"
#include "Particle/ParticleManager.h"
#include "Spread/SpreadManager.h"
#include "Seed/SeedManager.h"
#include "Logging/Logger.h"
#include "Logging/ScreenText.h"
#include "Time/Time.h"
#include "EntityList.h"
#include "Game/Clock.h"
#include "Helpers/PhysicsHelpers.h"
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
static constexpr float SPIN_ROTATION_SPEED = 0.05f;
static constexpr float ITEM_ROTATION_SPEED = 0.1f;
static constexpr float SLOPE_ROTATION_SPEED = 0.02f;
static constexpr float ATTACK_ROTATION_SPEED = 0.025f;
static constexpr float JUMP_CHARGE_ROTATION_SPEED = 0.015f;
static constexpr float AIR_ROTATION_SPEED = 0.01f;
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

static constexpr int MAX_ATTACK_CHARGE = 75;
static constexpr int STRONG_CHARGE_THRESH = 25;
static constexpr int ATTACK_STARTUP = 2;
static constexpr int ATTACK_ACTIVE = 13;
static constexpr int ATTACK_COOLDOWN = 10;
static constexpr int ATTACK_TIME = ATTACK_STARTUP + ATTACK_ACTIVE + ATTACK_COOLDOWN;

static constexpr int ACTIVATE_COOLDOWN_LENGTH = 35;
static constexpr float ACTIVATE_ROTATION_SPEED = 0.015f;
static constexpr float ACTIVATE_END_MAX_ROTATION_SPEED = 0.25f;
static constexpr float ACTIVATE_END_MIN_ROTATION_SPEED = 0.006f;

static constexpr float MIN_HOMING_VERTICAL_DELTA = 30;
static constexpr float MAX_HOMING_DISTANCE = 150;

static constexpr float FAST_THRESHOLD = 60.0f;

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
    attackActiveTimer_ = ATTACK_TIME;

    SetFlag(EF_SendPush, true);
    SetFlag(EF_RecievePush, true);
    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_StickToGround, true);
    SetFlag(EF_DownStickOnly, true);
    SetFlag(EF_AlignToGround, true);
    SetFlag(EF_UseVelocity, true);
    SetFlag(EF_UseSkeleton, true);
    SetFlag(EF_Interpolate, true);
    SetFlag(EF_SendHits, true);
    SetFlag(EF_RecieveHits, true);
    SetFlag(EF_RecieveKnockback, true);
    SetFlag(EF_HurtFaceForward, true);
    SetFlag(EF_CaptureSeed, true);
    SetFlag(EF_UseTrail, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("sk_char");
    skeleton_ = resourceManager.GetSkeleton("sk_char");

    for (int i = 0; i < 8; i++) {
        materials_[i].shadowShader = resourceManager.GetShader("vs_skeletal_s", "fs_depth_s");
        materials_[i].castShadows = true;
        materials_[i].specularProperties = SpecularFresnelProperties::Default();
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
    materials_[SHIRT].specularProperties.specularBrightness = 0.0f;

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

    hitbox_.radius = 6.0f;
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

    homingTarget_ = nullptr;

    chargingActivate_ = false;
    activateCooldown_ = 0;
    activateChargeAmount_ = 0;
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

    // Desired movement
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

    constexpr float MIN_TRACKING_DIST = 50.0f;
    constexpr float MIN_TRACKING_DIST2 = MIN_TRACKING_DIST * MIN_TRACKING_DIST;
    for (int i = 0; i < EntityList::MAX; i++) {
        Entity& entity = (*entities_)[i]; 
        if (!entity.alive_) continue;
        if (!entity.GetFlag(EF_Trackable)) continue;

        float dist2 = distance2(transform_.position, entity.transform_.position);
        if (dist2 > MIN_TRACKING_DIST2) continue;

        vec3 directionToEntity = entity.transform_.position - transform_.position;
        directionToEntity.y = 0.0f;
        directionToEntity = normalize(directionToEntity);

        quat rotation;
        if (dot(directionToEntity, desiredMovement) > 0.5f && length(desiredMovement) > 0.001f) {
            rotation = quatLookAtRH(desiredMovement, Transform::worldUp);
            quat desiredTrackRotation = quatLookAtRH(directionToEntity, Transform::worldUp);
            rotation = slerp(rotation, desiredTrackRotation, 0.25f);
            desiredMovement = rotation * transform_.worldForward;
            break;
        }
    }

    speedEmtter_->active_ = speed_ > FAST_THRESHOLD && onGround_;
    spinEmitter_->active_ = false;
    slopeEmitter_->active_ = false;

    // Homing attack search
    if (inputs_->startActivate && homingTarget_ == nullptr) {
        vec3 planarPos = vec3(transform_.position.x, 0.0f, transform_.position.z);
        vec3 planarVel = vec3(velocity_.x, 0.0f, velocity_.z);
        float planarVelMag = length(planarVel);
        float closestDistance = INFINITY;
        bool foundTarget = false;
        for (int i = 0; i < 128; i++) {
            Entity* entity = &((*entities_)[i]);
            if (!entity->alive_) continue;
            if (entity == this) continue;
            if (!entity->GetFlag(EF_Homeable)) continue;
            vec3 target = entity->GetTarget();
            vec3 targetPlanarPos = vec3(target.x, 0.0f, target.z);

            // We get the airtime to the target...
            float airtimeToTarget = GetAirtime(2.0f, velocity_.y, transform_.position.y, target.y);

            // ...and determine how far we can travel in our current velocity based
            // on that airtime...
            float maxDistanceForAirtime = planarVelMag * airtimeToTarget + 150.0f;
            float maxDistanceForAirtime2 = maxDistanceForAirtime * maxDistanceForAirtime;
            
            // ...so we can only target entities witihn that range
            float distToTarget2 = distance2(planarPos, targetPlanarPos);
            if (distToTarget2 > maxDistanceForAirtime2) continue;

            vec3 direction = normalize(planarVel);
            vec3 directionToTarget = normalize(targetPlanarPos - planarPos);
            if (dot(direction, directionToTarget) < 0.45f && distToTarget2 > 75.0f * 75.0f) continue;

            if (distToTarget2 < closestDistance) {
                homingTarget_ = entity;
                closestDistance = distToTarget2;
                foundTarget = true;
            }
        }
        if (foundTarget)
            planarVelMagPreHoming_ = planarVelMag; 
    }
    if (onGround_)
        homingTarget_ = nullptr;

    // Activation charge, cooldown, and use
    /*
    if (inputs_->startActivate && !chargingAttack_ && currency_->stocks_ > 0)
        chargingActivate_ = true;

    if (inputs_->releaseActivate)
        chargingActivate_ = false;

    if (chargingActivate_)
        activateChargeAmount_++;
    else if (activateChargeAmount_ != 0) {
        bool activated = false;
        for (int i = 0; i < EntityList::MAX; i++) {
            Entity& entity = (*entities_)[i];
            if (!entity.alive_) continue;
            if (entity.activator_.requiredStocks <= 0) continue;
            if (currency_->stocks_ < entity.activator_.requiredStocks) continue;

            float distance = distance2(transform_.position, entity.activator_.position);
            if (distance < entity.activator_.radius * entity.activator_.radius) {
                entity.DoActivate();
                activated = true;
                currency_->stocks_ -= entity.activator_.requiredStocks;
                break;
            }
        }
        if (moveLength > 0.0f) {
            if (speed_ < MAX_SPEED * 0.6f)
                speed_ = MAX_SPEED * 0.6f;

            // float yVel = velocity_.y;
            // velocity_ = desiredMovement * speed_;
            // velocity_.y = yVel;
            // transform_.rotation = quatLookAtRH(normalize(desiredMovement), Transform::worldUp);
        }
        if (!activated)
            currency_->stocks_--; 
        if (onGround_)
            velocity_.y -= 100.0f;
        activateChargeAmount_ = 0;
        activateCooldown_ = ACTIVATE_COOLDOWN_LENGTH;
    }

    if (activateCooldown_ > 0)
        activateCooldown_--;
        */

    // Attack charge and hitbox timing
    if (inputs_->startAttack && !chargingActivate_)
        chargingAttack_ = true;

    if (inputs_->releaseAttack || attackChargeAmount_ > MAX_ATTACK_CHARGE)
        chargingAttack_ = false;

    if (chargingAttack_ && attackActiveTimer_ == ATTACK_TIME)
        attackChargeAmount_++;
    else if (attackChargeAmount_ != 0) {
        lastAttackChargeAmount_ = attackChargeAmount_;
        attackChargeAmount_ = 0;
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

    // MoveMode determination
    moveMode_ = MM_Default;
    if (stun_) {
        moveMode_ = MM_Stun;
    }
    else if (homingTarget_ != nullptr) {
        moveMode_ = MM_Homing;
    }
    else if (activateCooldown_ > 0) {
        moveMode_ = MM_ActivateEnd;
    }
    else if (!onGround_) {
        moveMode_ = MM_Air;
    }
    else if (chargingActivate_) {
        moveMode_ = MM_Activate;
    }
    else if (attackActiveTimer_ < ATTACK_TIME) {
        moveMode_ = MM_Attack;
    }
    else if (attackChargeAmount_ != 0) {
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

    // Friction and acceleration
    float frictionLerp = 1.0f - (min(speed_, FRICTION_CAP) - MIN_SPEED) / (FRICTION_CAP - MIN_SPEED);
    float friction = lerp(MIN_FRICTION, MAX_FRICTION, frictionLerp);
    float speedDecay = 1.0f - friction;
    float acceleration = ((speed_ / speedDecay) - speed_);

    // Bonus speed calculation
    vec3 planarVelocity = vec3(velocity_.x, 0.0f, velocity_.z);
    if (length(planarVelocity) > 0.0f)
        velocity_ -= normalize(planarVelocity) * bonus_;
    speed_ = length(planarVelocity) - bonus_;

    // Groun stick velocity
    if (homingTarget_ == nullptr) {
        if (!onGround_)
            velocity_.y -= 2.0f;
        else if (moveMode_ == MM_Slope)
            velocity_.y -= 32.0f;
        else if (moveMode_ == MM_Attack)
            velocity_.y -= 16.0f;
        else if (moveMode_ == MM_Activate)
            velocity_.y -= 24.0f;
        else
            velocity_.y -= 6.0f;
    }

    // Movement calculation
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

        case MM_Activate: {
            quat initialRotation = transform_.rotation;
            if (length(planarVelocity) > 0.1f)
                initialRotation = quatLookAtRH(normalize(planarVelocity), Transform::worldUp);

            quat desiredRotation = initialRotation;

            if (length(desiredMovement) > 0.001f) 
               desiredRotation = quatLookAtRH(normalize(desiredMovement), Transform::worldUp);

            transform_.rotation= slerp(transform_.rotation, desiredRotation, ACTIVATE_ROTATION_SPEED);
            vec3 travelDirection = slerp(initialRotation, desiredRotation, ACTIVATE_ROTATION_SPEED) * Transform::worldForward;
            
            velocity_.x = travelDirection.x * speed_;
            velocity_.z = travelDirection.z * speed_;
            break;
        }

        case MM_ActivateEnd: {
            float endTime = (float)activateCooldown_ / ACTIVATE_COOLDOWN_LENGTH; 
            float rotationSpeed = lerp(ACTIVATE_END_MIN_ROTATION_SPEED, ACTIVATE_END_MAX_ROTATION_SPEED, endTime);
            quat initialRotation = transform_.rotation;
            if (length(planarVelocity) > 0.1f)
                initialRotation = quatLookAtRH(normalize(planarVelocity), Transform::worldUp);

            quat desiredRotation = initialRotation;

            if (length(desiredMovement) > 0.001f) 
               desiredRotation = quatLookAtRH(normalize(desiredMovement), Transform::worldUp);

            transform_.rotation= slerp(transform_.rotation, desiredRotation, rotationSpeed);
            vec3 travelDirection = slerp(initialRotation, desiredRotation, rotationSpeed) * Transform::worldForward;
            
            velocity_.x = travelDirection.x * speed_;
            velocity_.z = travelDirection.z * speed_;
            if (onGround_)
                velocity_.y -= 100.0f * endTime;
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

        case MM_Homing: {
            if (homingTarget_ != nullptr) {
                vec3 vectorToTarget = normalize(homingTarget_->GetTarget() - transform_.position);
                velocity_ = vectorToTarget * 500.0f;
            }
            break;
        }
    }
    planarVelocity = vec3(velocity_.x, 0.0f, velocity_.z);

    // Bonus speed decay and application
    bonus_ = bonus_ * 0.985f;
    if (length(planarVelocity) > 0.0f)
        velocity_ += normalize(planarVelocity) * bonus_;
    
    // Attack hitbox assignemnt
    if (lastAttackChargeAmount_ < STRONG_CHARGE_THRESH) {
        hitbox_.knocback = planarVelocity;
        hitbox_.knocback.y = max(velocity_.y + 60.0f, 35.0f);
        hitbox_.hitlag = 4;
        hitbox_.diStrength = 0.15f;
        hitbox_.hitType = Hitbox::Normal;
    }
    else {
        hitbox_.knocback = planarVelocity * 1.25f;
        hitbox_.knocback.y = max(velocity_.y + 60.0f, 35.0f);
        hitbox_.hitlag = 6;
        hitbox_.diStrength = 0.05f;
        hitbox_.hitType = Hitbox::Strong;
    }

    // Stun cancellation
    if (onGround_ && stun_) {
        stun_ = false;
    }

    // Animation determination
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
    else if (attackChargeAmount_ > 0) 
        animation = 5;
    else if (moveMode_ == MM_Spin)
        animation = 3;
    else if (moveMode_ == MM_Slope)
        animation = 2;
    else if (moveLength > 0.0f)
        animation = 1;

    if (animation != animIndex_)
        ChangeAnimation(animation, transitionLength);

    // Tilting
    float desiredTilt = 0.0f;
    if (!stun_) {
        if (moveLength > 0.001f)
            desiredTilt = dot(desiredMovement / moveLength, transform_.GetRightVector()) * 0.35f;
        tilt_ = lerp(tilt_, desiredTilt, 0.05f);
    }
    else
        tilt_ = 0.0f;

    // Ground trace distance assignment
    traceDistance_ = std::max(-velocity_.y * GlobalTime::TIMESTEP, 0.25f);

    // Spread removal
    if (speed_ > FAST_THRESHOLD) {
        Cone removeCone;
        removeCone.direction = normalize(-velocity_);
        removeCone.position = transform_.position;

        removeCone.distance = (speed_ / MAX_SPEED) * 32.0f;

        float removeAngleRatio = (speed_ - FAST_THRESHOLD) / MAX_SPEED + 50.0f;
        removeAngleRatio = clamp(removeAngleRatio, 0.0f, 1.0f);
        removeCone.angle = lerp(1.0f, 0.85f, removeAngleRatio);

        spreadManager_->RemoveSpread(removeCone, this, true);
    }

    SCREENLINE(1, std::to_string(speed_));
    SCREENLINE(2, std::to_string(length(velocity_)));
}

void Player::RenderUpdate() {
    speedEmtter_->transform_ = renderTransform_;
    speedEmtter_->transform_.position += velocity_ * GlobalTime::GetDeltaTime();
    spinEmitter_->transform_ = renderTransform_;
    slopeEmitter_->transform_ = renderTransform_;
    slopeEmitter_->transform_.position += velocity_ * GlobalTime::GetDeltaTime();

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
    clock_->AddTime();
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
        if (!onGround_)
            velocity_.y = 120.0f;
        else
            velocity_.y = 30.0f;
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

void Player::EndHoming() {
    if (homingTarget_ == nullptr) return;
    homingTarget_ = nullptr;
    
    quat cameraPlanarRotation = quat(vec3(0.0f, camera_->lookX_, 0.0f));
    vec3 cameraPlanarForward = cameraPlanarRotation * Transform::worldForward;
    vec3 cameraPlanarRight = cameraPlanarRotation * Transform::worldRight;
    vec3 desiredMovement = cameraPlanarForward * inputs_->forwardInput + cameraPlanarRight * inputs_->sideInput;
    vec3 planarDir = desiredMovement;//normalize(vec3(velocity_.x, 0.0f, velocity_.z));
    planarDir *= planarVelMagPreHoming_;
    velocity_ = vec3(planarDir.x, velocity_.y, planarDir.z);
}

void Player::Boost(float boost, float retention) {
    vec3 planarVelocity = vec3(velocity_.x, 0.0f, velocity_.z);
    velocity_ += normalize(planarVelocity) * boost;
    bonus_ = boost * (1.0f - retention);
}
