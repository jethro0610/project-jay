#pragma once
#include "Rendering/Bone.h"

class Skeleton;

class AnimationController {
public:
    Transform* transform_;
    Transform* lastTransform_;
    Skeleton* skeleton_;
    Pose pose_;
    Pose renderPose_; 

    int prevAnimationIndex_;
    int animationIndex_;

    float time_;
    float prevAnimationTime_; 
    float transitionTime_;
    float transitionLength_;

    void SetAnimationIndex(int index, float transitionLength = 0.35f);
    void Update();
};
