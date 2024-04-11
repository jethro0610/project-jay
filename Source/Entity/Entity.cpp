#include "Entity.h"
#include "Time/Time.h"
#include "Terrain/Terrain.h"
#include "Logging/Logger.h"
#include <glm/gtx/compatibility.hpp>
using namespace glm;

void Entity::Construct(
    Camera& camera,
    EntityList& entities,
    Inputs& inputs,
    Level& level,
    SeedManager& seedManager,
    SpreadManager& spreadManager,
    Terrain& terrain
) {
    camera_ = &camera;
    entities_ = &entities;
    inputs_ = &inputs;
    level_ = &level;
    seedManager_ = &seedManager;
    spreadManager_ = & spreadManager;
    terrain_ = &terrain;
}

void Entity::Init(
    Entity::InitArgs args
) {
    alive_ = true;
    destroy_ = false; 
    flags_ = 0; 
    
    lastTransform_ = transform_;
    renderTransform_ = transform_;
    velocity_ = {};

    traceDistance_ = 1.0f;
    onGround_ = false;
    groundHeight_ = 0.0f;
    groundNormal_ = {};

    model_ = nullptr;
    for (int i = 0; i < Model::MAX_MESHES_PER_MODEL; i++)
        materials_[i] = {};

    skeleton_ = nullptr;
    pose_ = {};
    renderPose_ = {};
    animIndex_ = 0;
    prevAnimIndex_ = 0;
    prevAnimTime_ = 0; 
    animTime_ = 0;
    transitionTime_ = 0.0f;
    transitionLength_ = 0.0f;

    pushbox_ = {};
    hitbox_ = {};
    hurtbox_ = {};
    stun_ = false;
    hitlag_ = 0;
    hurtCooldown_ = 0;

    #ifdef _DEBUG
    DBG_selected_ = false;
    DBG_collider_.radius = 1.0f;
    DBG_collider_.top = 1.0f;
    DBG_collider_.bottom = 1.0f;
    DBG_persistView_ = false;
    #endif
}

void Entity::SetFlag(Entity::Flag flag, bool enable) {
    uint32_t bit = 1UL << flag; 

    if (enable)
        flags_ |= bit;
    else
        flags_ &= ~bit;
}

bool Entity::GetFlag(Entity::Flag flag) {
    return (flags_ & 1UL << flag); 
}

void Entity::BaseUpdate() {
    bool useVelocity = GetFlag(EF_UseVelocity);

    if (hitlag_ == 0 && GetFlag(EF_GroundCheck)) {
        lastGroundNormal_ = groundNormal_;
        groundNormal_ = terrain_->GetNormal(transform_.position + velocity_ * GlobalTime::TIMESTEP);
        groundHeight_ = terrain_->GetHeight(transform_.position + velocity_ * GlobalTime::TIMESTEP);
        float distanceToSurface = transform_.position.y - groundHeight_;
        if (distanceToSurface < traceDistance_ && !skipGroundCheck_)
            onGround_ = true;
        else 
            onGround_ = false;

        if (GetFlag(EF_StickToGround)) {
            if (!skipGroundCheck_ && onGround_) {
                if (useVelocity) 
                    velocity_.y = -distanceToSurface / GlobalTime::TIMESTEP;
                else
                    transform_.position.y = groundHeight_;
            }
            else if (transform_.position.y  < groundHeight_)
                transform_.position.y = groundHeight_;
        }
    }

    if (hitlag_ == 0 && velocity_.y < 0.0f)
        skipGroundCheck_ = false;

    if (hitlag_ == 0 && useVelocity)
        transform_.position += velocity_ * GlobalTime::TIMESTEP;

    if ((hitlag_ == 0 || initHitlag_) && GetFlag(EF_UseSkeleton)) {
        int prevAnimFramerate = skeleton_->GetFramerate(prevAnimIndex_);
        int curAnimFramerate = skeleton_->GetFramerate(animIndex_);
        int framerate = transitionTime_ < transitionLength_ ? 
            std::lerp(prevAnimFramerate, curAnimFramerate, clamp(transitionTime_ / transitionLength_, 0.0f, 1.0f)) :
            curAnimFramerate;

        if (transitionTime_ >= transitionLength_ || transitionLength_ == 0.0f) {
            skeleton_->GetPose(
                pose_,
                animIndex_,
                animTime_,
                transform_,
                lastTransform_
            );
        }
        else {
            skeleton_->GetBlendedPose(
                pose_,
                prevAnimIndex_,
                animIndex_,
                prevAnimTime_,
                animTime_,
                transitionTime_ / transitionLength_,
                transform_,
                lastTransform_
            );
        }

        int frame = floor(animTime_ * 60.0f);
        int framesTillUpate = 60 / max(framerate, 1);

        if (frame % framesTillUpate == 0)
            renderPose_ = pose_;

        animTime_ += GlobalTime::TIMESTEP;
        prevAnimTime_ += GlobalTime::TIMESTEP;
        transitionTime_ += GlobalTime::TIMESTEP;
    }

    if (hitlag_ != 0)
        hitlag_--;

    if (hitlag_ == 0 && hurtCooldown_ > 0)
        hurtCooldown_--;

    initHitlag_ = false;
}

void Entity::CalculateBasePose() {
    if (!GetFlag(EF_UseSkeleton)) return;
    skeleton_->GetBasePose(pose_);
    renderPose_ = pose_;
}

void Entity::BaseRenderUpdate(float interpTime) {
    if (GetFlag(EF_Interpolate)) {
        renderTransform_ = Transform::Lerp(
            lastTransform_,
            transform_,
            interpTime
        );

        if (onGround_ && GetFlag(EF_AlignToGround)) {
            vec3 up = lerp(lastGroundNormal_, groundNormal_, interpTime);
            quat delta = rotation(Transform::worldUp, up);
            renderTransform_.rotation = delta * renderTransform_.rotation;
        }
    }
    else {
        renderTransform_ = transform_;
        if (onGround_ && GetFlag(EF_AlignToGround)) {
            quat delta = rotation(Transform::worldUp, groundNormal_);
            renderTransform_.rotation = delta * renderTransform_.rotation;
        }
    }

    
    if (hitlag_ > 0 && stun_) {
        renderTransform_.position.x += pow(sin(GlobalTime::GetTime() * 100.0f + 8.0f), 2.0f) * 0.5f;
        renderTransform_.position.y += pow(sin(GlobalTime::GetTime() * 100.0f + 16.0f), 2.0f) * 0.5f;
        renderTransform_.position.z += pow(sin(GlobalTime::GetTime() * 100.0f + 32.0f), 2.0f) * 0.5f;
    }
}

void Entity::ChangeAnimation(int index, float transitionLength) {
    prevAnimIndex_ = animIndex_;
    animIndex_ = index; 
    prevAnimTime_ = animTime_;
    animTime_ = 0.0f;

    transitionLength_ = transitionLength;
    transitionTime_ = 0.0f;
}
