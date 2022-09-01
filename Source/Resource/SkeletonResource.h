#pragma once
#include <string>
#include <vector>
#include "../Types/Transform.h"

#define MAX_JOINT_CHILDREN 8
#define SKELETON_HEADER_SIZE sizeof(uint8_t)
class Joint {
public:
    Transform localTransform_;
    glm::mat4 inverseBindMatrix_;
    int8_t childIndices_[MAX_JOINT_CHILDREN];
};

class SkeletonResource {
public:
    SkeletonResource(std::string skeletonName);
    std::vector<Joint> joints_;
    void CalculateBasePoseJointTransforms(glm::mat4* outputJointTransforms) const;

private:
    void CalculateBasePoseJointTransforms_Recursive(int currentJointIndex, glm::mat4* outputJointTransforms) const;
};

