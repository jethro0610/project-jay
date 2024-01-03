#include "AnimationController.h"
#include "Rendering/Skeleton.h"
#include "Time/Time.h"

void AnimationController::CalculatePose() {
    int prevAnimationFramerate = skeleton_->GetFramerate(prevAnimationIndex_);
    int curAnimationFramerate = skeleton_->GetFramerate(animationIndex_);

    int framerate = transitionTime_ < transitionLength_ ? 
        std::lerp(
            prevAnimationFramerate, 
            curAnimationFramerate, 
            std::clamp(transitionTime_ / transitionLength_, 0.0f, 1.0f)
        ) :
        curAnimationFramerate;

    if (transitionTime_ >= transitionLength_ || transitionLength_ == 0.0f) {
        skeleton_->GetPose(
            pose_,
            animationIndex_,
            time_,
            *transform_,
            *lastTransform_ 
        );
    }
    else {
        skeleton_->GetBlendedPose(
            pose_,
            prevAnimationIndex_,
            animationIndex_,
            prevAnimationTime_,
            time_,
            transitionTime_ / transitionLength_,
            *transform_,
            *lastTransform_
        );
        transitionTime_ += GlobalTime::TIMESTEP;
    }
    
    int frame = floor(time_ * 60.0f);
    int framesTillUpate = 60 / framerate;

    if (frame % framesTillUpate == 0)
        renderPose_ = pose_;
}

void AnimationController::SetAnimationIndex(int index, float transitionLength) {
    if (index == animationIndex_) return;

    prevAnimationIndex_ = animationIndex_;
    animationIndex_ = index; 
    prevAnimationTime_ = time_;
    time_ = 0.0f;

    transitionLength_ = transitionLength;
    transitionTime_ = 0.0f;
}
