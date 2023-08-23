#include "../Helpers/Assert.h"
#include "Skeleton.h"
#include "../Logging/Logger.h"
#include "../Logging/ScreenText.h"
#include "../Game/Time.h"
#include <glm/gtx/string_cast.hpp>

using namespace glm;

Transform Skeleton::GetAnimatedLocalBoneTransform(
    const Animation& animation, 
    float time, 
    int boneIndex
) const {
    int floorTime = floorf(time * KEYFRAMES_PER_SECOND);
    float lerpTime = (time * KEYFRAMES_PER_SECOND) - floorTime;

    int beforeIndex = floorTime;
    int afterIndex = beforeIndex + 1;

    beforeIndex = beforeIndex % animation.keyframes.size();
    afterIndex = afterIndex % animation.keyframes.size();

    Transform beforeTransform = animation.keyframes[beforeIndex].boneTransforms[boneIndex];
    Transform afterTransform =  animation.keyframes[afterIndex].boneTransforms[boneIndex];

    return Transform::Lerp(beforeTransform, afterTransform, lerpTime);
}

void Skeleton::GetAnimationPose_Recursive(
    const Animation& animation, 
    float time,
    Pose& pose, 
    int boneIndex
) const {
    const mat4& parentTransform = pose[boneIndex];

    for (int child : bones_[boneIndex].children) {
        mat4 childLocalTransform = GetAnimatedLocalBoneTransform(animation, time, child).ToMatrix();
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
    pose[0] = GetAnimatedLocalBoneTransform(animation, time, 0).ToMatrix();
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
