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
typedef vector_const<glm::mat4, MAX_JOINTS> JointTransforms;
class Skeleton {
public:
    SkeletonJoints joints_;
    std::vector<Animation> animations_;

    void GetBasePose_Recursive(JointTransforms& jointTransforms, int jointIndex) const;
    void GetBasePose(JointTransforms& jointTransforms) const;

    void GetAnimationPose_Recursive(const Animation& animation, float time, JointTransforms& jointTransforms, int jointIndex) const;
    void GetAnimationPose(const Animation& animation, float time, JointTransforms& jointTransforms) const;
    void GetAnimationPose(int animationIndex, float time, JointTransforms& jointTransforms) const;

    Transform GetAnimatedLocalJointTransform(const Animation& animation, float time, int jointIndex) const;
};
