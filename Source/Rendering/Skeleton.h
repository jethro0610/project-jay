#pragma once
#include <array>
#include <glm/mat4x4.hpp>
#include <unordered_map>
#include <vector_const.h>
#include "Animation.h"
#include "RenderConstants.h"
#include "../Types/Transform.h"

struct Bone {
    glm::mat4 inverseBindMatrix;
    vector_const<int, MAX_BONE_CHILDREN> children;
};

typedef vector_const<Bone, MAX_BONES> Bones;
typedef vector_const<glm::mat4, MAX_BONES> Pose;
class Skeleton {
public:
    Bones bones_;
    std::vector<Animation> animations_; // Animations are stored directly on skeleton.
                                        // This makes the relationship clearer and they
                                        // will deallocate with the skeleton

    void GetAnimationPose_Recursive(const Animation& animation, float time, Pose& pose, int boneIndex) const;
    void GetAnimationPose(const Animation& animation, float time, Pose& pose) const;
    void GetAnimationPose(int animationIndex, float time, Pose& pose) const;
    Transform GetAnimatedLocalBoneTransform(const Animation& animation, float time, int boneIndex) const;
};
