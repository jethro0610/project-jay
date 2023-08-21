#include "../Helpers/Assert.h"
#include "Skeleton.h"
#include "../Logging/Logger.h"
#include <glm/gtx/string_cast.hpp>

using namespace glm;

void Skeleton::GetBasePose_Recursive(JointTransforms& jointTransforms, int jointIndex) const {
    const mat4& parentTransform = jointTransforms[jointIndex];

    for (int child : joints_[jointIndex].children) {
        mat4 childLocalTransform = joints_[child].localTransform.GetWorldMatrix();
        jointTransforms[child] = parentTransform * childLocalTransform;
        GetBasePose_Recursive(jointTransforms, child);
    }
}

void Skeleton::GetBasePose(JointTransforms& jointTransforms) const {
    jointTransforms.resize(joints_.size());
    jointTransforms[0] = joints_[0].localTransform.GetWorldMatrix();
    GetBasePose_Recursive(jointTransforms, 0);
    for (int i = 0; i < joints_.size(); i++)
        jointTransforms[i] = jointTransforms[i] * joints_[i].inverseBindMatrix;
}

Transform Skeleton::GetAnimatedLocalJointTransform(
    const Animation& animation, 
    float time, 
    int jointIndex
) const {
    int index = floorf(time * KEYFRAMES_PER_SECOND);
    index = index % animation.keyframes.size();
    return animation.keyframes[index].transform[jointIndex];
}

void Skeleton::GetAnimationPose_Recursive(
    const Animation& animation, 
    float time,
    JointTransforms& jointTransforms, 
    int jointIndex
) const {
    const mat4& parentTransform = jointTransforms[jointIndex];

    for (int child : joints_[jointIndex].children) {
        mat4 childLocalTransform = GetAnimatedLocalJointTransform(animation, time, child).GetWorldMatrix();
        jointTransforms[child] = parentTransform * childLocalTransform;
        GetAnimationPose_Recursive(animation, time, jointTransforms, child);
    }
}

void Skeleton::GetAnimationPose(
    const Animation& animation, 
    float time, 
    JointTransforms& jointTransforms
) const {
    jointTransforms.resize(joints_.size());
    jointTransforms[0] = GetAnimatedLocalJointTransform(animation, time, 0).GetWorldMatrix();
    GetAnimationPose_Recursive(animation, time, jointTransforms, 0);
    for (int i = 0; i < joints_.size(); i++)
        jointTransforms[i] = jointTransforms[i] * joints_[i].inverseBindMatrix;
}

void Skeleton::GetAnimationPose(
    int animationIndex, 
    float time, 
    JointTransforms& jointTransforms
) const {
    GetAnimationPose(animations_[animationIndex], time, jointTransforms); 
}
