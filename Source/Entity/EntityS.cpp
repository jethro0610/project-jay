#include "EntityS.h"
#include "Time/Time.h"
#include "Terrain/Terrain.h"
#include "Logging/Logger.h"
using namespace glm;

void EntityS::Construct(
    Camera& camera,
    Inputs& inputs,
    Terrain& terrain
) {
    camera_ = &camera;
    inputs_ = &inputs;
    terrain_ = &terrain;
}

void EntityS::Init(
    EntityS::InitArgs args
) {
    flags_ = 0; 
    transform_ = {};
    lastTransform_ = {};
    renderTransform_ = {};
    velocity_ = {};

    traceDistance_ = 1.0f;
    onGround_ = false;
    groundHeight_ = 0.0f;
    groundNormal_ = {};

    model_ = nullptr;
    for (int i = 0; i < Model::MAX_MESHES_PER_MODEL; i++)
        materials_[i] = nullptr;

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
}

void EntityS::SetFlag(EntityS::Flag flag, bool enable) {
    uint32_t bit = 1UL << flag; 

    if (enable)
        flags_ |= bit;
    else
        flags_ &= ~bit;
}

bool EntityS::GetFlag(EntityS::Flag flag) {
    return (flags_ & 1UL << flag); 
}

void EntityS::BaseUpdate() {
    bool useVelocity = GetFlag(EF_UseVelocity);

    if (GetFlag(EF_GroundCheck)) {
        groundNormal_ = terrain_->GetNormal(transform_.position + velocity_ * GlobalTime::TIMESTEP);
        groundHeight_ = terrain_->GetHeight(transform_.position + velocity_ * GlobalTime::TIMESTEP);
        float distanceToSurface = transform_.position.y - groundHeight_;
        if (distanceToSurface < traceDistance_)
            onGround_ = true;
        else 
            onGround_ = false;

        if (onGround_ && GetFlag(EF_StickToGround) && !noStickThisUpdate_) {
            if (useVelocity) 
                velocity_.y = -distanceToSurface / GlobalTime::TIMESTEP;
            else
                transform_.position.y = groundHeight_;
        }

        if (velocity_.y < 0.0f)
            noStickThisUpdate_ = false;
    }

    if (useVelocity)
        transform_.position += velocity_ * GlobalTime::TIMESTEP;

    if (GetFlag(EF_UseSkeleton)) {
        animTime_ += GlobalTime::TIMESTEP;
        prevAnimTime_ += GlobalTime::TIMESTEP;

        int prevAnimFramerate = skeleton_->GetFramerate(prevAnimIndex_);
        int curAnimFramerate = skeleton_->GetFramerate(animIndex_);
        int framerate = transitionTime_ < transitionLength_ ? 
            std::lerp(prevAnimFramerate, curAnimFramerate, std::clamp(transitionTime_ / transitionLength_, 0.0f, 1.0f)) :
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
            transitionTime_ += GlobalTime::TIMESTEP;
        }

        int frame = floor(animTime_ * 60.0f);
        int framesTillUpate = 60 / framerate;

        if (frame % framesTillUpate == 0)
            renderPose_ = pose_;
    }
}

void EntityS::BaseRenderUpdate(float interpTime) {
    if (GetFlag(EF_Interpolate)) {
        renderTransform_ = Transform::Lerp(
            lastTransform_,
            transform_,
            interpTime
        );
    }
    else
        renderTransform_ = transform_;
}

void EntityS::ChangeAnimation(int index, float transitionLength) {
    prevAnimIndex_ = index;
    animIndex_ = index; 
    prevAnimTime_ = animTime_;
    animTime_ = 0.0f;

    transitionLength_ = transitionLength;
    transitionTime_ = 0.0f;
}
