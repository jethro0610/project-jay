#include "../Helpers/Assert.h"
#include "Skeleton.h"
#include "../Logging/Logger.h"
#include "../Logging/ScreenText.h"
#include "../Game/Time.h"
#include <glm/gtx/string_cast.hpp>

using namespace glm;

Transform Skeleton::GetLocalBoneTransform(
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

    Transform beforeTransform = animation.keyframes[beforeIndex].pose[boneIndex];
    Transform afterTransform =  animation.keyframes[afterIndex].pose[boneIndex];

    return Transform::Lerp(beforeTransform, afterTransform, lerpTime);
}

void Skeleton::GetPose_Recursive(
    Pose& pose, 
    const Animation& animation, 
    float time,
    int boneIndex
) const {
    const Transform& parentTransform = pose[boneIndex];

    for (int child : bones_[boneIndex].children) {
        // Optimizaiton: Can this be calculated this with transforms directly?
        // Right now the calculation goes from Transform -> Two Matrices -> Transform
        mat4 childLocalTransform = GetLocalBoneTransform(animation, time, child).ToMatrix();
        pose[child] = parentTransform.ToMatrix() * childLocalTransform;
        GetPose_Recursive(pose, animation, time, child);
    }
}

void Skeleton::GetPose(
    Pose& pose,
    const Animation& animation, 
    float time
) const {
    pose.resize(bones_.size());
    pose[0] = GetLocalBoneTransform(animation, time, 0);
    GetPose_Recursive(pose, animation, time, 0);
}

void Skeleton::GetPose(
    Pose& pose, 
    int animationIndex, 
    float time
) const {
    GetPose(pose, animations_[animationIndex], time); 
}

void Skeleton::GetGPUPose(
    GPUPose& pose,
    const Animation& animation, 
    float time
) const {
    Pose worldPose;
    GetPose(worldPose, animation, time);

    pose.resize(bones_.size());
    for (int i = 0; i < bones_.size(); i++)
        pose[i] = worldPose[i].ToMatrix() * bones_[i].inverseBindMatrix;
}

void Skeleton::GetGPUPose(
    GPUPose& pose, 
    int animationIndex, 
    float time
) const {
    GetGPUPose(pose, animations_[animationIndex], time);
}
