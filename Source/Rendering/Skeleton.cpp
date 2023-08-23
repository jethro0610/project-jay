#include "../Helpers/Assert.h"
#include "Skeleton.h"
#include "../Logging/Logger.h"
#include <glm/gtx/string_cast.hpp>

using namespace glm;

Transform Skeleton::GetAnimatedLocalBoneTransform(
    const Animation& animation, 
    float time, 
    int boneIndex
) const {
    int index = floorf(time * KEYFRAMES_PER_SECOND);
    index = index % animation.keyframes.size();
    return animation.keyframes[index].boneTransforms[boneIndex];
}

void Skeleton::GetAnimationPose_Recursive(
    const Animation& animation, 
    float time,
    Pose& pose, 
    int boneIndex
) const {
    const mat4& parentTransform = pose[boneIndex];

    for (int child : bones_[boneIndex].children) {
        mat4 childLocalTransform = GetAnimatedLocalBoneTransform(animation, time, child).GetWorldMatrix();
        pose[child] = parentTransform * childLocalTransform;
        GetAnimationPose_Recursive(animation, time, pose, child);
    }
}

void Skeleton::GetAnimationPose(
    const Animation& animation, 
    float time, 
    Pose& pose
) const {
    pose.resize(bones_.size());
    pose[0] = GetAnimatedLocalBoneTransform(animation, time, 0).GetWorldMatrix();
    GetAnimationPose_Recursive(animation, time, pose, 0);
    for (int i = 0; i < bones_.size(); i++)
        pose[i] = pose[i] * bones_[i].inverseBindMatrix;
}

void Skeleton::GetAnimationPose(
    int animationIndex, 
    float time, 
    Pose& pose
) const {
    GetAnimationPose(animations_[animationIndex], time, pose); 
}
