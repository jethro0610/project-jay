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

void Skeleton::GetWorldPose(
    Pose& pose,
    const Transform& transform,
    int animationIndex,
    float time
) const {
    pose.resize(bones_.size());
    pose[0] = transform.ToMatrix() * GetLocalBoneTransform(animations_[animationIndex], time, 0).ToMatrix();
    GetPose_Recursive(pose, animations_[animationIndex], time, 0); 
}

void Skeleton::ComputeRibbonChain(Pose& pose, const Pose& desiredPose, int startBone, int endBone, float deltaTime) const {
    std::array<float, MAX_BONES> desiredDistances;
    int numChainBones = (endBone - startBone) + 1;
    pose[startBone] = desiredPose[startBone];

    // OPTIMIZATION: Precompute the bone distances
    for (int i = 1; i < numChainBones; i++) {
        const int boneIndex = i + startBone;
        desiredDistances[boneIndex] = distance(desiredPose[boneIndex].position, desiredPose[boneIndex - 1].position);
    }

    // Calculate bone positions
    for (int i = 1; i < numChainBones; i++) {
        const int boneIndex = i + startBone;
            
        vec3 fromParent = pose[boneIndex].position - pose[boneIndex - 1].position;
        if (length(fromParent) > desiredDistances[boneIndex])
            pose[boneIndex].position = pose[boneIndex - 1].position + (normalize(fromParent) * desiredDistances[boneIndex]);

        vec3 velocity = desiredPose[boneIndex].position - pose[boneIndex].position;
        float maxVelocity = length(velocity);

        float scalar = (numChainBones - 1) - i;
        scalar /= numChainBones - 2;
        scalar = std::powf(scalar, 2.0f);
        scalar = mix(0.05f, 0.95f, scalar);

        velocity *= 100.0f * scalar * deltaTime;
        if (length(velocity) > maxVelocity)
            velocity = normalize(velocity) * maxVelocity;

        pose[boneIndex].position += velocity;
    }

    // Calculate bone rotations, the tail is skipped since it's only used
    // as reference for the last deforming bone
    for (int i = 1; i < numChainBones - 1; i++) {
        const int boneIndex = i + startBone;

        vec3 a = normalize(desiredPose[boneIndex + 1].position - desiredPose[boneIndex].position);
        vec3 b = normalize(pose[boneIndex + 1].position - pose[boneIndex].position);
        quat deltaDir = rotation(a, b);

        pose[boneIndex].rotation = deltaDir * desiredPose[boneIndex].rotation;
    }
}

void Skeleton::GetWorldPose(
    Pose& pose, 
    const Transform& transform, 
    const RibbonChainList& ribbonChainList, 
    float deltaTime,
    int animationIndex, 
    float time
) const {
    Pose desiredPose;
    GetWorldPose(desiredPose, transform, animationIndex, time);

    std::array<bool, MAX_BONES> isRibbon;
    isRibbon.fill(false);

    for (const std::pair<int, int>& ribbonChain : ribbonChainList) {
        ComputeRibbonChain(pose, desiredPose, ribbonChain.first, ribbonChain.second, deltaTime);
        for (int i = ribbonChain.first; i <= ribbonChain.second; i++)
            isRibbon[i] = true;
    }

    for (int i = 0; i < pose.size(); i++) {
        if (!isRibbon[i])
            pose[i] = desiredPose[i];
    }
}

void Skeleton::WorldPoseToGPUPose(GPUPose& gpuPose, const Pose& worldPose) const {
    gpuPose.resize(bones_.size());
    for (int i = 0; i < bones_.size(); i++)
        gpuPose[i] = worldPose[i].ToMatrix() * bones_[i].inverseBindMatrix;
}
