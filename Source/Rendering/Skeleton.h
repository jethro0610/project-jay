#pragma once
#include <array>
#include <glm/mat4x4.hpp>
#include <unordered_map>
#include <utility>
#include <vector_const.h>
#include "Animation.h"
#include "Bone.h"
#include "Ribbon.h"
#include "SkeletonConstants.h"
#include "../Types/Transform.h"
using namespace SkeletonConstants;

typedef vector_const<std::pair<int, int>, MAX_RIBBON_CHAINS> RibbonChainList; 

class Skeleton {
public:
    Bones bones_;
    Ribbons ribbons_;
    std::vector<Animation> animations_; // Animations are stored directly on skeleton.
                                        // This makes the relationship clearer and they
                                        // will deallocate with the skeleton

    void GetPose_Recursive(Pose& pose, const Animation& animation, float time, int boneIndex) const;
    void GetPose(Pose& pose, const Animation& animation, float time) const;
    void GetPose(Pose& pose, int animationIndex, float time) const;

    void GetPose(
        Pose& pose, 
        int animationIndex, 
        float time, 
        const Transform& transform, 
        const Transform& lastTransform
    ) const;

    void GetBlendedPose(
        Pose& pose, 
        int prevAnimationIndex, 
        int animationIndex,
        float prevTime, 
        float time,
        float blend,
        const Transform& transform, 
        const Transform& lastTransform
    ) const;
    void PoseToGPUPose(GPUPose& gpuPose, const Pose& pose) const;

    Transform GetLocalBoneTransform(const Animation& animation, float time, int boneIndex) const;

    int GetFramerate(int animationIndex) const { return animations_[animationIndex].framerate; }

private:
    void ComputeRibbonChain(
        Pose& pose, 
        const Pose& desiredPose, 
        const RibbonDesc& ribbon, 
        const Transform& transform, 
        const Transform& lastTransform,
        float speedInfluence
    ) const;
};
