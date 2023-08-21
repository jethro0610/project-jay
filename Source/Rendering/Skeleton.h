#pragma once
#include <array>
#include <glm/mat4x4.hpp>
#include <unordered_map>
#include <vector_const.h>
#include "Animation.h"
#include "RenderConstants.h"
#include "../Types/Transform.h"

struct Joint {
    Transform localTransform;
    glm::mat4 inverseBindMatrix;
    vector_const<int, MAX_JOINT_CHILDREN> children;

};

typedef vector_const<Joint, MAX_JOINTS> SkeletonJoints;
class Skeleton {
public:
    SkeletonJoints joints_;
    std::vector<Animation> animations_;

    void GetBasePose(std::array<glm::mat4, MAX_JOINTS>& jointTransforms) const;
    void GetAnimationPose(const Animation& animation, float time, std::array<glm::mat4, MAX_JOINTS>& jointTransforms) const;
    void GetAnimationPose(int animationIndex, float time, std::array<glm::mat4, MAX_JOINTS>& jointTransforms) const;
    Transform GetLocalJointTransformAtTime(const Animation& animation, float time, int jointIndex) const;
};
