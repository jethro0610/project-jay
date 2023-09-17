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
    int keyframe = (int)floorf(time * KEYFRAMES_PER_SECOND) % animation.keyframes.size();
    return animation.keyframes[keyframe].pose[boneIndex];
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

void Skeleton::GetPose(
    Pose& pose,
    int animationIndex,
    float time,
    const Transform& transform,
    const Transform& lastTransform
) const {
    Pose desiredPose;
    desiredPose.resize(bones_.size());
    desiredPose[0] = GetLocalBoneTransform(animations_[animationIndex], time, 0).ToMatrix();
    GetPose_Recursive(desiredPose, animations_[animationIndex], time, 0); 

    std::array<bool, MAX_BONES> isRibbon;
    isRibbon.fill(false);

    for (const RibbonDesc& ribbon : ribbons_) {
        ComputeRibbonChain(
            pose, 
            desiredPose, 
            ribbon, 
            transform,
            lastTransform,
            animations_[animationIndex].speedInfluence
        );
        for (int i = ribbon.start; i <= ribbon.end; i++)
            isRibbon[i] = true;
    }

    for (int i = 0; i < pose.size(); i++) {
        if (!isRibbon[i])
            pose[i] = desiredPose[i];
    }
}

void Skeleton::GetBlendedPose(
    Pose& pose,
    int prevAnimationIndex,
    int animationIndex,
    float prevTime,
    float time,
    float blend,
    const Transform& transform,
    const Transform& lastTransform
) const {
    Pose prevDesiredPose;
    prevDesiredPose.resize(bones_.size());
    prevDesiredPose[0] = GetLocalBoneTransform(animations_[prevAnimationIndex], prevTime, 0).ToMatrix();
    GetPose_Recursive(prevDesiredPose, animations_[prevAnimationIndex], prevTime, 0); 

    Pose desiredPose;
    desiredPose.resize(bones_.size());
    desiredPose[0] = GetLocalBoneTransform(animations_[animationIndex], time, 0).ToMatrix();
    GetPose_Recursive(desiredPose, animations_[animationIndex], time, 0); 

    for (int i = 0; i < bones_.size(); i++)
        desiredPose[i] = Transform::Lerp(prevDesiredPose[i], desiredPose[i], blend);

    std::array<bool, MAX_BONES> isRibbon;
    isRibbon.fill(false);

    for (const RibbonDesc& ribbon : ribbons_) {
        ComputeRibbonChain(
            pose, 
            desiredPose, 
            ribbon, 
            transform,
            lastTransform,
            animations_[animationIndex].speedInfluence
        );
        for (int i = ribbon.start; i <= ribbon.end; i++)
            isRibbon[i] = true;
    }

    for (int i = 0; i < pose.size(); i++) {
        if (!isRibbon[i])
            pose[i] = desiredPose[i];
    }
}

void Skeleton::ComputeRibbonChain(
    Pose& pose, 
    const Pose& desiredPose, 
    const RibbonDesc& ribbon, 
    const Transform& transform, 
    const Transform& lastTransform,
    float speedInfluence
) const {
    std::array<float, MAX_BONES> desiredDistances;
    int numChainBones = (ribbon.end - ribbon.start) + 1;
    pose[ribbon.start] = desiredPose[ribbon.start];

    mat4 modelMatrix = transform.ToMatrix();
    mat4 inverseModelMatrix = inverse(modelMatrix);
    mat4 lastModelMatrix = lastTransform.ToMatrix();

    Pose worldPose = pose;
    Pose desiredWorldPose = desiredPose;
    vec3 additive = (transform.position - lastTransform.position) * (1.0f - speedInfluence);

    // OPTIMIZATION: Precompute the bone distances
    for (int i = 0; i < numChainBones; i++) {
        const int boneIndex = i + ribbon.start;
        worldPose[boneIndex] = lastModelMatrix * worldPose[boneIndex].ToMatrix();
        worldPose[boneIndex].position += additive;
        desiredWorldPose[boneIndex] = modelMatrix * desiredWorldPose[boneIndex].ToMatrix();
        desiredDistances[boneIndex] = distance(desiredPose[boneIndex].position, desiredPose[boneIndex - 1].position);
    }

    // Calculate bone positions
    for (int i = 1; i < numChainBones; i++) {
        const int boneIndex = i + ribbon.start;
            
        vec3 fromParent = worldPose[boneIndex].position - worldPose[boneIndex - 1].position;
        if (length(fromParent) > desiredDistances[boneIndex])
            worldPose[boneIndex].position = worldPose[boneIndex - 1].position + (normalize(fromParent) * desiredDistances[boneIndex]);

        vec3 velocity = desiredWorldPose[boneIndex].position - worldPose[boneIndex].position;
        float maxVelocity = length(velocity);
        
        float scalar = (numChainBones - 1) - i;
        scalar /= numChainBones - 2;
        scalar = std::pow(scalar, ribbon.tailPower);
        scalar = mix(ribbon.tailRatio, 1.0f, scalar);

        velocity *= ribbon.returnSpeed * scalar;
        worldPose[boneIndex].position += velocity;
    }

    // Calculate bone rotations, the tail is skipped since it's only used
    // as reference for the last deforming bone
    for (int i = 1; i < numChainBones; i++) {
        const int boneIndex = i + ribbon.start;

        vec3 a = normalize(desiredWorldPose[boneIndex + 1].position - desiredWorldPose[boneIndex].position);
        vec3 b = normalize(worldPose[boneIndex + 1].position - worldPose[boneIndex].position);
        quat deltaDir = rotation(a, b);

        worldPose[boneIndex].rotation = deltaDir * desiredWorldPose[boneIndex].rotation;
        pose[boneIndex] = inverseModelMatrix * worldPose[boneIndex].ToMatrix();
    }
}

void Skeleton::PoseToGPUPose(GPUPose& gpuPose, const Pose& pose) const {
    gpuPose.resize(bones_.size());
    for (int i = 0; i < bones_.size(); i++)
        gpuPose[i] = pose[i].ToMatrix() * bones_[i].inverseBindMatrix;
}
