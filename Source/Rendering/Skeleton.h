#pragma once
#include <array>
#include <glm/mat4x4.hpp>
#include <unordered_map>
#include <utility>
#include <vector_const.h>
#include "Animation.h"
#include "Bone.h"
#include "RenderConstants.h"
#include "../Types/Transform.h"

const int MAX_SNAKE_CHAINS = 4;
typedef vector_const<std::pair<int, int>, MAX_SNAKE_CHAINS> SnakeChainList; 

class Skeleton {
public:
    Bones bones_;
    std::vector<Animation> animations_; // Animations are stored directly on skeleton.
                                        // This makes the relationship clearer and they
                                        // will deallocate with the skeleton

    void GetPose_Recursive(Pose& pose, const Animation& animation, float time, int boneIndex) const;
    void GetPose(Pose& pose, const Animation& animation, float time) const;
    void GetPose(Pose& pose, int animationIndex, float time) const;

    void GetWorldPose(Pose& pose, const Transform& transform, int animationIndex, float time) const;
    void GetWorldPose(
        Pose& pose, 
        const Transform& transform, 
        const SnakeChainList& snakeChainList, 
        float deltaTime,
        int animationIndex, 
        float time
    ) const;
    void WorldPoseToGPUPose(GPUPose& gpuPose, const Pose& worldPose) const;

    Transform GetLocalBoneTransform(const Animation& animation, float time, int boneIndex) const;
};
